/// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

/// Includes
#include "../Schema/llrx_generated.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "flatbuffers/util.h"

/// Main
int main (int argc, char* argv[]) {
	auto file = File::getCurrentWorkingDirectory().getChildFile("test.mid");
	
	std::unique_ptr<InputStream> stream(file.createInputStream());
	std::unique_ptr<MidiFile> midi(new MidiFile());
	midi->readFrom(*stream);
	
	std::cout << "Tracks: " << midi->getNumTracks() << std::endl;
	midi->convertTimestampTicksToSeconds();
	
	flatbuffers::FlatBufferBuilder builder(1024);

	std::vector<flatbuffers::Offset<Entity>> entities_vector;
	
	for (int i = 0; i < midi->getNumTracks(); i++) {
		auto track = midi->getTrack(i);
		std::cout << " Track: " << i << std::endl;
		
		for (int j = 0; j < track->getNumEvents(); j++) {
			auto event = track->getEventPointer(j);
			
			if (event->message.isNoteOn()) {
				std::cout << "  Event: " << event->message.getDescription() << std::endl;
				std::cout << "   Time: " << event->message.getTimeStamp() << std::endl;
				
				auto entity = CreateEntity(builder, event->message.getNoteNumber(), event->message.getTimeStamp(),
																	 event->message.getChannel());
				entities_vector.push_back(entity);
			}
		}
	}
	
	auto entities = builder.CreateVector(entities_vector);
	auto name = builder.CreateString("Test");
	
	LlrxBuilder llrxBuilder(builder);
	llrxBuilder.add_amount(static_cast<int32_t>(entities_vector.size()));
	llrxBuilder.add_name(name);
	llrxBuilder.add_entities(entities);
	
	auto orc = llrxBuilder.Finish();
	builder.Finish(orc);
	
	auto buf = builder.GetBufferPointer();
	int size = builder.GetSize();
	
	flatbuffers::SaveFile("test.llrx", (const char *) buf, size, true);
	
	//// Test reading
	std::string readbuf;
	flatbuffers::LoadFile("test.llrx", true, &readbuf);
	
	auto readOrc = GetLlrx(readbuf.c_str());
	std::cout << "Name: " << readOrc->name()->c_str() << std::endl;
	std::cout << "Amount: " << readOrc->amount() << std::endl;
	
	for (auto it = readOrc->entities()->begin(); it != readOrc->entities()->end(); ++it) {
		// access element as *it
		std::cout << "Entities:" << std::endl;
		
		std::cout << " Note:" << it->note() << std::endl;
		std::cout << " Time:" << it->time() << std::endl;
		std::cout << " Delta:" << it->delta() << std::endl;
		
		// any code including continue, break, return
	}
}
