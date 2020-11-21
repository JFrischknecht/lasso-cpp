// Header file for functionality on the in-sample fit 

#ifndef FIT_H
#define FIT_H

#include "matrix.h"
#include <vector>

double r_squared(const vec &y, const vec &fitted);

double rmse(const vec &y, const vec &fitted);
vec rmse(const vec &y, const matrix &fitted);

double mae(const vec &y, const vec &fitted);

void print_statistics(const vec &y, const vec &fitted);

#endif