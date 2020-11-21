// Main Lasso algorithm

#include "matrix.h"
#include "stats.h"
#include "fit.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib> // for std::exit()

double soft_threshold(const double x, const double lambda)
{
  // Soft-thresholding operator to update a single value x, given a penalty parameter lambda
  if (x > lambda)
  {
    return x - lambda;
  }
  else if (x < (-lambda))
  {
    return x + lambda;
  }
  else
  {
    return 0;
  }
}

vec compute_partial_residual(const matrix &x, const matrix &y, const vec &beta,
                             const double j)
{
  // Computes the partial residual using all but the j'th column of x

  // Intitialise partial residual
  vec partial_residual{};
  partial_residual.resize(nrow(x));

  // Compute partial residual for every obseration
  for (int i{0}; i < nrow(x); i++)
  {
    double residual_sum{0};

    // Compute the residual sum
    for (int k{0}; k < ncol(x); k++)
    {
      if (k != j)
      {
        residual_sum += x[k][i] * beta[k];
      }
    }

    partial_residual[i] = y[0][i] - residual_sum;
  }

  return partial_residual;
}

vec coordinate_descent_lasso(const matrix &x, const matrix &y,
                             const vec &beta_start, const double lambda,
                             const double max_iters = 1000,
                             const double convergence_criterion = 0.0001)
{
  // Given the data, a penalty parameter lambda, a maximum number of iterations, and a convergence criterion apply the coordinate descent algorithm for LASSO using a warm start and active set iteration:

  // Note that the matrix of predictors is supposed to be standardized and the outcome vector is supposed to be demeaned. The matrix of predictors should NOT contain an intercept.

  // Initialise the vector of coefficients with the "warm-start-values":
  vec beta(beta_start.begin(), beta_start.end());

  // Initialise the 'active set of predictors'
  std::vector<bool> active(ncol(x), true);

  // Run until convergence or until a maximum number of iterations is reached
  for (int iter{0}; iter < max_iters; iter++)
  {
    // Initialise variable to count how many coefficients are updated in this iteration beyond the convergence criterion
    int divergence_counter{0};

    // Cycle through all (active) covariates
    for (int j{0}; j < ncol(x); j++)
    {
      if (active[j])
      {
        // Initialise/Compute the partial residual
        vec partial_residual{compute_partial_residual(x, y, beta, j)};

        // Compute the univariate OLS-coefficient
        double ols_coef{inner_product(x[j], partial_residual) / nrow(x)};

        // Compute the updated coefficient with the soft-thresholding operator
        double beta_j_update{soft_threshold(ols_coef, lambda)};

        // If the update is larger than the convergence criterion increment the divergence counter
        if (std::abs(beta[j] - beta_j_update) > convergence_criterion)
        {
          divergence_counter++;
        }

        // Perform the update
        beta[j] = beta_j_update;

        // Check if coefficient is non-zero (Soft-thresholding operator can shrink to exactly zero, at which point the covariate is no longer considered active)
        if (beta[j] == 0.0)
        {
          active[j] = false;
        }
      }
    }

    // Early exit if convergence is reached
    if (divergence_counter == 0)
    {
      return beta;
    }
  }

  std::cout << "Maximum number of iterations reached without convergence!"
            << " Returning results of the last iteration.\n";

  return beta;
}

vec create_lambda_grid(const matrix &x, const matrix &y,
                       const double epsilon = 0.0001, const double n = 100)
{
  // Given the data, create a grid of lambda values, by first determining a maximum lamda, which will shrink all coefficients to zero, and then construct a sequence of n values on the log-scale.

  // Note that the matrix of predictors is supposed to be standardized and the outcome vector is supposed to be demeaned. The matrix of predictors should NOT contain an intercept.

  // Find the largest inner-product between the outcome and any covariate:
  vec inner_products(ncol(x), 0);

  for (int j{0}; j < ncol(x); j++)
  {
    inner_products[j] = std::abs(inner_product(matrix_to_vector(y), x[j]));
  }

  // Lambda_max = max_j |<x_j, y> | / N
  // Lambda_min = epsilon * Lambda_max
  double lambda_max{(*std::max_element(inner_products.begin(),
                                       inner_products.end())) /
                    nrow(x)};

  double lambda_min{epsilon * lambda_max};

  // Lambda sequence - n values between Lambda_max and Lambda_min (included),
  // Evenly spaced on a log-axis.
  vec lambda_sequence{sequence(std::log(lambda_min), std::log(lambda_max), n)};

  // Transform back to levels (from logs):
  for (int k{0}; k < n; k++)
  {
    lambda_sequence[k] = std::exp(lambda_sequence[k]);
  }

  // Sort in descending order
  std::sort(lambda_sequence.rbegin(), lambda_sequence.rend());

  return lambda_sequence;
}

matrix pathwise_coordinate_descent(const matrix &x, const matrix &y,
                                   const vec &beta_start, const vec lambdas)
{
  // Given the data, a starting point for the beta-vector and a decreasing sequence of penalty paramaters, run the coordinate descent algorithm for each penalty parameter using warm starts, i.e. by using the previous solution as starting point

  // Note that the matrix of predictors is supposed to be standardized and the outcome vector is supposed to be demeaned. The matrix of predictors should NOT contain an intercept.

  // Matrix to store coefficients
  matrix betas{allocate_matrix(lambdas.size(), ncol(x))};

  for (int i{0}; i < lambdas.size(); i++)
  {
    if (i == 0)
    {
      // In first iteration use supplied beta-vector as starting point
      betas[i] = coordinate_descent_lasso(x, y, beta_start, lambdas[i]);
    }
    else
    {
      // Warm-start: Use betas from previous iteration as starting point
      betas[i] = coordinate_descent_lasso(x, y, betas[i - 1], lambdas[i]);
    }
  }

  return betas;
}

double recover_intercept(const vec &beta, const vec &mean_x,
                         const double mean_y)
{
  // Given an estimated coefficient vector, aswell as the original means of the predictors and the outcome, estimate the intercept of the lasso model
  return (mean_y - inner_product(beta, mean_x));
}

vec recover_intercept(const matrix &beta, const vec &mean_x,
                      const double mean_y)
{
  // Given an a sequence of estimated coefficient vectors, aswell as the original means of the predictors and the outcome, estimate the intercepts of each lasso model
  vec intercepts{};

  for (auto i : beta)
  {
    intercepts.push_back(recover_intercept(i, mean_x, mean_y));
  }

  return intercepts;
}

vec add_intercept(const vec &betas, const double intercept)
{
  // Creates a vector containing the estimated intercept as first element, followed by the coefficients of all predictors
  vec coefs{};
  coefs.push_back(intercept);

  for (auto i : betas)
  {
    coefs.push_back(i);
  }

  return coefs;
}

vec predict(const matrix &x, const double intercept, const vec &beta)
{
  // Given the standardized design matrix, the estimated intercept, aswell as the previously computed coefficient vector beta, estimate the fitted values of the lasso-model

  vec fitted_values{};
  fitted_values.resize(nrow(x));

  for (int i{0}; i < nrow(x); i++)
  {
    for (int j{0}; j < ncol(x); j++)
    {
      fitted_values[i] += x[j][i] * beta[j];
    }

    // Add intercept
    fitted_values[i] += intercept;
  }

  return fitted_values;
}

matrix predict(const matrix &x, const vec intercepts, const matrix &betas)
{
  // Given the standardized design matrix, the estimated intercepts, aswell as a sequence of coefficient vectors, estimate the fitted values of each associated lasso model

  matrix fitted_values{allocate_matrix(ncol(betas), nrow(x))};

  for (int j{0}; j < ncol(fitted_values); j++)
  {
    fitted_values[j] = predict(x, intercepts[j], betas[j]);
  }

  return fitted_values;
}

vec lasso_cv(const matrix &x, const matrix &y, const vec &lambdas,
             const int n_folds)
{
  // Given the original data, a sequence of penalty parameters and the number of folds k, do the following: For each lambda value in the grid: Apply k-fold cross-validation to estimate the cross-validated RMSE of each lasso model (one model for each lambda value)

  // Create folds
  matrix_int folds{create_folds(nrow(x), n_folds)};

  // Initialise the vector of coefficients
  vec beta_null(ncol(x), 0.0);

  // Container to store the CV-RMSE's
  matrix cv_rmse{allocate_matrix(n_folds, lambdas.size())};

  // Iterate through all folds
  for (int k{0}; k < n_folds; k++)
  {
    // Produce test and training set
    matrix x_train{create_training_set(x, folds, k)};
    matrix y_train{create_training_set(y, folds, k)};

    matrix x_test{create_test_set(x, folds, k)};
    matrix y_test{create_test_set(y, folds, k)};

    // Demean the outcome and standardize the design matrix
    // (We do this within the fold to minimize leakage)
    matrix y_train_c{center(y_train)};
    matrix x_train_s{standardize(x_train)};
    matrix x_test_s{standardize(x_test)};

    // Run pathwise coordinate descent using warm-starts on the training data
    // (standardized X, centered Y)
    matrix betas_train{pathwise_coordinate_descent(x_train_s, y_train_c,
                                                   beta_null, lambdas)};

    // Recover the estimated intercepts for all models
    double mean_y_train{mean(matrix_to_vector(y_train))};
    vec mean_x_train{mean(x_train)};
    vec intercepts_train{recover_intercept(betas_train,
                                           mean_x_train, mean_y_train)};

    // Estimate fitted values on the test set using the coefficients estimated on the training set, for all lambda-values
    matrix fitted_values_test{predict(x_test_s,
                                      intercepts_train,
                                      betas_train)};

    // Now evaluate the RMSE on this test set for all models
    cv_rmse[k] = rmse(matrix_to_vector(y_test), fitted_values_test);
  }

  // Now average the RMSE's across folds, to obtain the CV-RMSE for each lambda-parameter
  return rowmean(cv_rmse);
}
