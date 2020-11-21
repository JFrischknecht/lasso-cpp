// Header file for general statistical functions

#ifndef STATS_H
#define STATS_H

#include "matrix.h"
#include <vector>

double square(const double &x);
double sum(const vec &x);
double mean(const vec &x);
vec mean(const matrix &x);
vec rowmean(const matrix &x);
double sd(const vec &x);
vec sd(const matrix &x);
double inner_product(const vec &x, const vec &y);

vec center(const vec &x);
vec standardize(const vec &x);
matrix center(const matrix &x);
matrix standardize(const matrix &x);

vec sequence(const double start, const double end, const int length);
vec_int sequence(const int start, const int length);

int which_min(const vec &x);

vec subset(const vec &x, const vec_int &indices);
matrix subset(const matrix &x, const vec_int &indices);

matrix_int create_folds(const int length, const int k);

matrix create_test_set(const matrix &x, const matrix_int &folds,
                       const int active_fold);

matrix create_training_set(const matrix &x, const matrix_int &folds,
                           const int active_fold);
#endif