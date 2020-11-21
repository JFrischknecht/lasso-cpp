// Header file for main lasso algorithm 

#ifndef ALGORITHM_H
#define ALOGRITHM_H

#include "matrix.h"
#include "stats.h"

double soft_threshold(const double x, const double lambda);

vec compute_partial_residual(const matrix &x, const matrix &y,
                             const vec &beta, const double j);

vec coordinate_descent_lasso(const matrix &x, const matrix &y,
                             const vec &beta_start, const double lambda, 
                             const double max_iters = 1000, 
                             const double convergence_criterion = 0.0001);

vec create_lambda_grid(const matrix &x, const matrix &y,
                       const double epsilon = 0.0001, const double n = 100);

matrix pathwise_coordinate_descent(const matrix &x, const matrix &y,
                                   const vec &beta_start, const vec lambdas);

double recover_intercept(const vec &beta, const vec &mean_x,
                         const double mean_y);

vec recover_intercept(const matrix &beta, const vec &mean_x,
                      const double mean_y);

vec add_intercept(const vec &betas, const double intercept);

vec predict(const matrix &x, const double intercept, const vec &beta);
matrix predict(const matrix &x, const vec intercepts, const matrix &betas);

vec lasso_cv(const matrix &x, const matrix &y, const vec &lambdas,
             const int n_folds);

#endif