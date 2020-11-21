// Header-file for exporting functionality

#ifndef EXPORT_H
#define EXPORT_H

#include "matrix.h"

#include <string>
#include <fstream>
#include <vector>
#include <cstdlib> // for std::exit()
#include <iostream>

void write_csv(std::string file_name, vec fitted_values, std::string header)
{
  // Create an output filestream object
  std::ofstream export_file(file_name);

  // Print an error and exit if file not writeable
  if (!export_file)
  {
    std::cerr << file_name << " could not be opened for writing!"
              << std::endl;
    std::exit(2);
  }

  // Send the column name to the stream
  export_file << header << "\n";

  // Send fitted values to the stream
  for (int i = 0; i < fitted_values.size(); ++i)
  {
    export_file << fitted_values.at(i);

    // Only add a new line, if its not the last line
    if (i < (fitted_values.size() - 1))
    {
      export_file << "\n";
    }
  }

  // Close the file
  export_file.close();
}

#endif