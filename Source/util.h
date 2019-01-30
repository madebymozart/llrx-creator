/**
 * Copyright (c) 2019 Mozart Louis
 */

#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

/**
 * Assures the passed in string end matches the provided ending.
 *
 * @param value The full string we will be checking the ending too.
 * @param ending The ending we want to compare.
 * @return True of the value's ending is equal to the provided one, otherwise false.
 */
inline bool ends_with(std::string const& value, std::string const& ending) {
  if (ending.size() > value.size()) return false;
  return std::equal(value.rbegin(), value.rbegin() + ending.size(), ending.rbegin(),
                    ending.rend());
}

/**
 * Removes the file extension from the given string
 *
 * @param filename Value to remove the file extension from.
 * @param new_extension New extension to append to the string.
 * @return New string with new file extension. Will return original string if no '.'
 * character is found.
 */
inline std::string replace_extension(const std::string& filename,
                                     const std::string& new_extension) {
  size_t lastdot = filename.find_last_of(".");
  if (lastdot == std::string::npos) return filename;
  return filename.substr(0, lastdot).append(new_extension);
}

#endif  /// __UTIL_H__
