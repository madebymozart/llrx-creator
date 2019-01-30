/**
 * Copyright (c) 2017 Mozart Louis
 */

#ifndef __CREATOR_H__
#define __CREATOR_H__

 /**
  * JSON Related Keys
  */
constexpr auto __LLRX_OUTPUT__ = "output";    /// Output directory where llrx files will be saved;
constexpr auto __LLRX_FILE_NAMES__ = "files"; /// Array of midi file names that will be converted into LLRX files;

/**
 * System includes
 */
#include <string>
#include <list>
#include <queue>

 /**
  * Local includes
  */
#include "schema/llrx_generated.h"
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
	 * @param path Path to the folder containing midi files
	 */
	explicit Creator();

	/**
	 * Destructor
	 */
	~Creator();

	/**
	 * Using the json config file, this function will generate all the llrx files. Each llrx file will also be validated
	 * in the validate function
	 */
	void createLlrxFile(const std::string& file);
private:
	/**
	 * Creates the entitiy vector
	 */
	EntityVector createEntities(flatbuffers::FlatBufferBuilder& builder, std::unique_ptr<MidiFile>& midi_file);

	/**
	 * Created the LLRX file
	 */
	LlrxFile createLlrx(flatbuffers::FlatBufferBuilder& builder, const std::string& name,
		const int32_t& amount, EntityVector& entities);

	/**
	 * Validates that the llrx file created and contains the same amount of entities as the midi file did
	 */
	bool validateLlrx(const std::string& llrx, const int32_t entity_amout);

	/// List of thread instances
	std::list<std::string> threads_;

	/// path to folder containing midi files
	std::string path;
};

#endif /// __CREATOR_H__

