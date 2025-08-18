#include "utils/csv_file_reader.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

std::vector<std::vector<std::string>> read_csv(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: could not open the file." << std::endl;
        return {};
    }

    std::vector<std::vector<std::string>> csvData;
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ','))
        {
            row.push_back(cell);
        }

        csvData.push_back(row);
    }

    file.close();

    return csvData;
}