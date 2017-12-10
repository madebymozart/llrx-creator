/// Copyright (c) 2017 Mozart Alexander Louis. All rights reserved.

/// Includes
#include "cmd/cmd.hpp"
#include "creator/creator.h"
#include "JuceHeader.h"

/// Configure the command-line arguments for LlrxCreator
void configure(cli::Parser &parser) {
	parser.set_optional<std::string>("c", "config", "config.json");
}

/// Main
int main (int argc, char* argv[]) {
	std::cout << std::endl << "###### LlrxCreator - A tool to convert tailered midi files to Llrx files for Lleeria";
	std::cout << std::endl << "###### Copyright (c) 2017 Mozart Louis" << std::endl;
	
	/// Create command line parser to handle all the command things
	cli::Parser parser(argc, argv);
	configure(parser);
	
	/// Read the json file and pass it to the main parser that will create llrx files. This default to config.json
	/// but can be overwrittern with the `-c` in a terminal
	const std::string json_file = parser.get<std::string>("c");
	std::cout << std::endl << "###### Reading \"" << json_file << "\"..." << std::endl;
	
	/// Run the creator with the json file to create the llrx files
	new Creator(json_file.c_str());
	
	/// Exit
	return 0;
}
