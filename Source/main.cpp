/// Copyright (c) 2019 Mozart Alexander Louis. All rights reserved.

/// Includes
#include <filesystem>
#include <iostream>
#include "cmd/cmd.hpp"
#include "creator/creator.h"
#include "util.h"

/// Configure the command-line arguments for LlrxCreator
void configure(cli::Parser& parser) {
  parser.set_required<std::string>("d", "directory", "",
                                   "Relative path to directory with containing files");
}

/// Main
int main(int argc, char* argv[]) {
  std::cout << std::endl
            << "###### LlrxCreator 1.1.0 - A tool to convert midi files to llrx files "
               "for Lleeria";
  std::cout << std::endl
            << "###### Copyright (c) 2019 Mozart Louis" << std::endl
            << std::endl;

  /// Create command line parser to handle all the command things
  cli::Parser parser(argc, argv);
  configure(parser);

  /// Read the json file and pass it to the main parser that will create llrx files. This
  /// default to config.json but can be overwrittern with the `-c` in a terminal
  auto path = parser.get<std::string>("d");
  const auto& creator = new Creator();

  /// Delete and remake the output directory
  std::filesystem::remove_all(__LLRX_OUTPUT__);
  std::filesystem::create_directory(__LLRX_OUTPUT__);

  /// Go through every file in the directory and try to create a llrx file with them.
  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    const auto name = entry.path().generic_string();
    if (ends_with(name, ".mid") || ends_with(name, ".midi"))
      creator->createLlrxFile(name);
  }

  /// Exit
  return 0;
}