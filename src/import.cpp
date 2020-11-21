// Data import functionality

#include "matrix.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>   // stringstream
#include <fstream>   // std::ifstream
#include <cstdlib>   // for std::exit()
#include <algorithm> // for std::remove

std::string ReplaceAll(std::string str, const std::string &from,
                       const std::string &to)
{
  // Copied from https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string

  // Replaces all substrings within a string
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos)
  {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
}

matrix read_csv(std::string file_name, int skip_lines = 1)
{
  // Reads a CSV-File containing numeric-only columns into a "matrix"

  // Create an input stream
  std::ifstream import_file{file_name};

  // Print an error and exit if not readable
  if (!import_file)
  {
    std::cerr << file_name << " could not be opened for reading"
              << std::endl;
    std::exit(2);
  }

  // Initialize counter for row, string to hold the line, a matrix to hold
  // all the values and a bool to indicate if the first row was successfully read
  int line_number{1};
  std::string line{};
  matrix values{};
  bool first_successful_read{true};

  while (getline(import_file, line))
  {
    // Import lines after skipping a user-supplied number of rows
    if (line_number > skip_lines)
    {
      // Initialize stringstream with the line that was read
      std::stringstream ss{line};

      // Extract contents of the string and make sure that after each comma ',' there is withespace afterwards, s.t. the cell-entries are properly separated below
      std::string s = ss.str();
      s = ReplaceAll(s, std::string(","), std::string(", "));
      ss.str(s);

      // String to hold the value of each cell
      std::string cell{};

      // Intialize column-counter
      int column_index{1};

      while (ss >> cell) // Stringstream breaks on WHITESPACE
      {
        // Resize matrix in first iteration
        if (first_successful_read)
        {
          values.resize(column_index);
        }

        // Remove any whitespace and ','
        cell.erase(std::remove(cell.begin(), cell.end(), ' '), cell.end());
        cell.erase(std::remove(cell.begin(), cell.end(), ','), cell.end());

        // Explicitly convert value to a double
        double cell_double{std::stod(cell)};

        // Save values in matrix
        values[(column_index - 1)].push_back(cell_double);

        // Update column index
        ++column_index;
      }

      // Set indicator to false after first run (and in all subsequent ones)
      first_successful_read = false;
    }
    ++line_number;
  }

  // Close the file stream
  import_file.close();

  // Check if all column vectors have the same size
  if (!is_rectangular(values))
  {
    std::cerr << "Error, not all columns of " << file_name
              << " have the same length!" << std::endl;
    std::exit(2);
  }

  // Check whether all column vectors have some variation
  if (!has_variation(values))
  {
    std::cerr << "Error, constant column(s) present in "
              << file_name << "!" << std::endl;
    std::exit(2);
  }

  std::cout << file_name << " successfully imported!"
            << "\n";

  return values;
}
