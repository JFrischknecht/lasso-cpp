// Header file for matrix functionality

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

// Type aliases: A matrix is a vector of vectors, i.e. a vector of column-vectors
using matrix = std::vector<std::vector<double>>;
using matrix_int = std::vector<std::vector<int>>;
using vec = std::vector<double>;
using vec_int = std::vector<int>;

// Forward declarations
matrix allocate_matrix(const int cols, const int rows);
matrix_int allocate_matrix_int(const int cols, const int rows);

bool is_rectangular(const matrix &x);
bool has_variation(const vec &x);
bool has_variation(const matrix &x);
vec matrix_to_vector(const matrix &x);

int nrow(const matrix &x);
int ncol(const matrix &x);

int nrow(const matrix_int &x);
int ncol(const matrix_int &x);

void print(const matrix &x);
void print(const matrix_int &x);

void print(const vec &x);
void print(const vec_int &x);

#endif