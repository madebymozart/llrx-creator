/// Copyright (c) 2017 Mozart Louis

/// Class
#include "creator.h"

/// Local includes
#include "flatbuffers/util.h"
#include "util.h"

Creator::Creator() {
	/// delete the output folder and
	const std::string o(__LLRX_OUTPUT__);
};

Creator::~Creator() = default;

void Creator::createLlrxFile(const std::string& file) {
	/// MidiFile instance. Using to std::unique pointer for local instance and self desturction.
	std::unique_ptr<MidiFile> midi(new MidiFile());
	std::unique_ptr<InputStream> stream;

	/// Get the file content using Juce and create the midi file for it
	auto juceFile = File::getCurrentWorkingDirectory().getChildFile(juce::String((file).c_str()));
	if (!juceFile.exists()) {
		std::cout << "###### " << file << ".mid does not exist..." << std::endl;
		return;
	}

	/// Stream the file to create a midi object instance
	stream.reset(juceFile.createInputStream());
	midi->readFrom(*stream);

	if (midi->getNumTracks() <= 2) {
		std::cout << "###### " << file << ".mid is corrupt... Re-export..." << std::endl;
		return;
	}
	midi->convertTimestampTicksToSeconds();

	/// Create flatbuffer instance of this thread
	flatbuffers::FlatBufferBuilder builder(1024);

	/// Create entities
	auto entities = createEntities(builder, midi);

	/// Create Llrx Orc and save the buffer to a file on disk
	int32_t amount = static_cast<int32_t>(entities.size());
	builder.Finish(createLlrx(builder, file, amount, entities));

	const auto llrx_file = std::string(__LLRX_OUTPUT__) + "/" + replace_extension(file, ".llrx");
	flatbuffers::SaveFile(llrx_file.c_str(), (const char *)builder.GetBufferPointer(),
		builder.GetSize(), true);

	/// Validate the the saved file is good and can be read back.
	if (validateLlrx(llrx_file, amount))
		std::cout << "###### " << file << ".llrx created and verified! Placed in " << llrx_file << std::endl;
	else std::cout << "###### " << file + ".llrx failed... could not verify " << llrx_file << " :[" << std::endl;
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
					event->message.getTimeStamp(), event->message.getChannel()));
		}
	}

	/// Return vector
	return vector;
}

LlrxFile Creator::createLlrx(flatbuffers::FlatBufferBuilder &builder, const std::string &name,
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

	/** auto readOrc = GetLlrx(readbuf.c_str());
	 std::cout << "Name: " << readOrc->name()->c_str() << std::endl;
	 std::cout << "Amount: " << readOrc->amount() << std::endl;

	 for (auto it = readOrc->entities()->begin(); it != readOrc->entities()->end(); ++it) {
	 // access element as *it
	 std::cout << "Entities:" << std::endl;

	 std::cout << " Note:" << it->note() << std::endl;
	 std::cout << " Time:" << it->time() << std::endl;
	 std::cout << " Delta:" << it->delta() << std::endl;

	 // any code including continue, break, return
	 } */

	return VerifyLlrxBuffer(verifier) && entity_amount == GetLlrx(readbuf.c_str())->amount();

}

