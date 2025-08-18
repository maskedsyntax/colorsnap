#pragma once

#include <string>
#include <vector>

// Reads a CSV file into a 2D vector of strings
// Returns an empty vector if the file could not be opened
std::vector<std::vector<std::string>> read_csv(const std::string& filename);
