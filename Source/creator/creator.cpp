/// Copyright (c) 2017 Mozart Louis

/// Class
#include "creator.h"

/// System Includes
#include <thread>

/// Local includes
#include "flatbuffers/util.h"

Creator::Creator(const char* json_config) {
	/// Open the json file
	std::ifstream file(json_config);
	
	/// copy content of the file to the json object
	if (!file.bad() && file.is_open()) {
		file >> j_;
		execute();
		
		/// Once execute is finished, prinr the done statment
		std::cout << std::endl << "###### Done!" << std::endl << std::endl;
	} else std::cout << "##### JSON Config \"" << json_config << "\" either does not exist, is malformed or corrupt :(";
	
	/// Close file
	file.close();
}

Creator::~Creator() { j_.clear(); }

void Creator::execute() {
	/// delete the output folder and
	const std::string o = j_[__LLRX_OUTPUT__];
	std::system(("if [ -d " + o + " ]; then rm -rf " + o + " && mkdir " + o + "; else mkdir " + o + "; fi").c_str());
	
	/// Get the array of file names and process them. Each file will be processed on a different thread to speed up
	/// creating all the files
	const auto& files = j_[__LLRX_FILE_NAMES__];
	std::cout << "###### Converting " + std::to_string(files.size()) + " midi file(s) to llrx file(s)";
	std::cout << std::endl << std::endl;
	
	for (const std::string& file : files) {
		/// Add this file name to the list of files currently being processed.
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		/// MidiFile instance. Using to std::unique pointer for local instance and self desturction.
		std::unique_ptr<MidiFile> midi(new MidiFile());
		std::unique_ptr<InputStream> stream;
		
		/// Get the file content using Juce and create the midi file for it
		auto juceFile = File::getCurrentWorkingDirectory().getChildFile(juce::String((file + ".mid").c_str()));
		if (!juceFile.exists()) {
			std::cout << "###### " << file << ".mid does not exist..." << std::endl;
			continue;
		}
		
		/// Stream the file to create a midi object instance
		stream.reset(juceFile.createInputStream());
		midi->readFrom(*stream);
		
		if (midi->getNumTracks() <= 2) {
			std::cout << "###### " << file << ".mid is corrupt... Re-export..." << std::endl;
			continue;
		}
		
		midi->convertTimestampTicksToSeconds();
		
		/// Create flatbuffer instance of this thread
		flatbuffers::FlatBufferBuilder builder(1024);
		
		/// Create entities
		auto entities = createEntities(builder, midi);
		
		/// Create Llrx Orc and save the buffer to a file on disk
		int32_t amount = static_cast<int32_t>(entities.size());
		builder.Finish(createLlrx(builder, file, amount, entities));
		
		const auto llrx_file = o + file + ".llrx";
		flatbuffers::SaveFile(llrx_file.c_str(), (const char *) builder.GetBufferPointer(),
													builder.GetSize(), true);
		
		/// Validate the the saved file is good and can be read back.
		if (validateLlrx(llrx_file, amount))
			std::cout << "###### " << file << ".llrx created and verified! Placed in " << llrx_file << std::endl;
		else std::cout << "###### "<<  file + ".llrx failed... could not verify " <<llrx_file << " :[" << std::endl;
	}
}

EntityVector Creator::createEntities(flatbuffers::FlatBufferBuilder& builder, std::unique_ptr<MidiFile>& midi_file) {
	/// Vector that holds all the entities
	EntityVector vector;
	
	/// Interate through all the tracks in the midi file and add all `noteOn` messages to the enenties vector
	for (int i = 0; i < midi_file->getNumTracks(); i++) {
		auto track = midi_file->getTrack(i);
		
		for (int j = 0; j < track->getNumEvents(); j++) {
			/// Retieve the event and put it in a unique pointer to avoid leaking
			auto event(track->getEventPointer(j));
			
			/// When the note is on, we will store that information in an entity and add it to the enitites vector
			if (event->message.isNoteOn())
				vector.push_back(CreateEntity(builder, event->message.getNoteNumber(),
																			event->message.getTimeStamp(),event-> message.getChannel()));
		}
	}
	
	/// Return vector
	return vector;
}

LlrxFile Creator::createLlrx(flatbuffers::FlatBufferBuilder &builder,const std::string &name,
														 const int32_t &amount, EntityVector &entities) {
	/// Create builder version of this variables
	auto b_name = builder.CreateString(name);
	auto b_entities = builder.CreateVector(entities);
	
	/// Create Llrx builder
	LlrxBuilder llrxBuilder(builder);
	llrxBuilder.add_amount(amount);
	llrxBuilder.add_name(b_name);
	llrxBuilder.add_entities(b_entities);
	
	/// Retrun LlrxFile Buffer
	return llrxBuilder.Finish();
}

bool Creator::validateLlrx(const std::string& llrx, const int32_t entity_amount) {
	/// Buffer that will contain the llrx data
	std::string readbuf;
	flatbuffers::LoadFile(llrx.c_str(), true, &readbuf);
	
	/// Validate this is a LlrxBuffer and is readable
	flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t *>(readbuf.c_str()), readbuf.length());
	return VerifyLlrxBuffer(verifier) and entity_amount == GetLlrx(readbuf.c_str())->amount();
}
