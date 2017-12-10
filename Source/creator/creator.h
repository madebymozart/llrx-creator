/**
 * Copyright (c) 2017 Mozart Louis
 */

#ifndef __CREATOR_H__
#define __CREATOR_H__

/**
 * JSON Related Keys
 */
#define __LLRX_OUTPUT__ "output"    /// Output directory where llrx files will be saved
#define __LLRX_FILE_NAMES__ "files" /// Array of midi file names that will be converted into LLRX files

/**
 * System includes
 */
#include <string>
#include <list>

/**
 * Local includes
 */
#include "json/json.hpp"
#include "llrx_generated.h"
#include "JuceHeader.h"

/**
 * Type alias for entities
 */
using EntityVector = std::vector<flatbuffers::Offset<Entity>>;
using LlrxFile = flatbuffers::Offset<Llrx>;

class Creator {
public:
	/**
	 * Constructor
	 *
	 * @param json_config JSON config file name
	 */
	explicit Creator(const char* json_config);
	
	/**
	 * Destructor
	 */
	~Creator();
	
	/**
	 * Using the json config file, this function will generate all the llrx files. Each llrx file will also be validated
	 * in the validate function
	 */
	void execute();
private:
	/**
	 * Creates the entitiy vector
	 */
	EntityVector createEntities(flatbuffers::FlatBufferBuilder& builder, std::unique_ptr<MidiFile>& midi_file);
	
	/**
	 * Created the LLRX file
	 */
	LlrxFile createLlrx(flatbuffers::FlatBufferBuilder& builder, const std::string& name,
											const int32_t& amount,EntityVector& entities);
	
	/**
	 * Validates that the llrx file created and contains the same amount of entities as the midi file did
	 */
	bool validateLlrx(const std::string& llrx, const int32_t entity_amout);
	
	/// List of thread instances
	std::list<std::string> threads_;
	
	/// Json parser using nlohmann
	nlohmann::json j_;
};

#endif /// __CREATOR_H__

