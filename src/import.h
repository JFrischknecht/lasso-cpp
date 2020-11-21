// Header-file for importing functionality

#ifndef IMPORT_H
#define IMPORT_H

#include "matrix.h"
#include <vector>
#include <string>

std::string ReplaceAll(std::string str, const std::string &from,
                       const std::string &to);

matrix read_csv(std::string file_name, int skip_lines = 1);

#endif