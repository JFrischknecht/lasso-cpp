// Matrix functionality

#include "matrix.h"

#include <iostream>
#include <vector>
#include <cstdlib> // for std::exit()

matrix allocate_matrix(const int cols, const int rows)
{
  // Create an uninitialized matrix object of given dimensions
  matrix new_matrix{};
  new_matrix.resize(cols);

  for (int j{0}; j < cols; j++)
  {
    new_matrix[j].resize(rows);
  }

  return new_matrix;
}

matrix_int allocate_matrix_int(const int cols, const int rows)
{
  // Create an uninitialized matrix object of given dimensions
  matrix_int new_matrix{};
  new_matrix.resize(cols);

  for (int j{0}; j < cols; j++)
  {
    new_matrix[j].resize(rows);
  }

  return new_matrix;
}

bool is_rectangular(const matrix &x)
{
  // Checks if a matrix object is rectangular
  for (int i{0}; i < x.size(); i++)
  {
    if (x[i].size() != x[0].size())
    {
      return false;
    }
  }
  return true;
}

bool has_variation(const vec &x)
{

  // Determines if a vector contains more than one unique value
  bool not_constant{false};

  for (int i{1}; i < x.size(); i++)
  {
    if (x[i] != x[i - 1])
    {
      not_constant = true;
      break;
    }
  }

  return not_constant;
}

bool has_variation(const matrix &x)
{

  // Determines whether all columns in a matrix have more than one unique value
  for (int j{0}; j < ncol(x); j++)
  {
    if (!has_variation(x[j]))
    {
      return false;
    }
  }

  return true;
}

vec matrix_to_vector(const matrix &x)
{
  // Converts a matrix with only one column to a vector
  if (x.size() != 1)
  {
    std::cerr << "Cannot convert matrix with more than 1 column to a vector!";
    std::exit(2);
  }
  else
  {
    return x[0];
  }
}

int nrow(const matrix &x)
{
  // Returns the number of rows a of a matrix
  return x[0].size();
}

int ncol(const matrix &x)
{
  // Returns the number of columns of a matrix
  return x.size();
}

int nrow(const matrix_int &x)
{
  // Returns the number of rows a of a matrix
  return x[0].size();
}

int ncol(const matrix_int &x)
{
  // Returns the number of columns of a matrix
  return x.size();
}

void print(const matrix &x)
{
  // Prints the element of a matrix
  for (int i{0}; i < nrow(x); ++i)
  {
    for (int j{0}; j < ncol(x); ++j)
    {
      std::cout << x[j][i] << "   ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void print(const matrix_int &x)
{
  // Prints the element of a matrix
  for (int i{0}; i < nrow(x); ++i)
  {
    for (int j{0}; j < ncol(x); ++j)
    {
      std::cout << x[j][i] << "   ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void print(const vec &x)
{
  // Prints the element of a vector
  for (int i{0}; i < x.size(); ++i)
  {
    std::cout << x[i] << " ";
  }
  std::cout << "\n";
}

void print(const vec_int &x)
{
  // Prints the element of a vector
  for (int i{0}; i < x.size(); ++i)
  {
    std::cout << x[i] << " ";
  }
  std::cout << "\n";
}