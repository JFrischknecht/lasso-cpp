// Statistical fit functions

#include "stats.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <iostream>
#include <cstdlib> // for std::exit()

double r_squared(const vec &y, const vec &fitted)
{
  // Computes the R^2 given a vector of outcomes and fitted values

  // Check if dimensions of inputs match
  if (y.size() != fitted.size())
  {
    std::cerr << "Outcome vector and fitted values need to have the same number of rows!";
    std::exit(2);
  }

  // Compute Sum of Squares total
  double sst{};
  double mu{mean(y)};

  vec squared_devs_mean{y};
  std::for_each(squared_devs_mean.begin(),
                squared_devs_mean.end(),
                [mu](double &y) {
                  y = square(y - mu);
                });
  sst = sum(squared_devs_mean);

  // Compute Sum of Squares Explained
  double sse{};
  vec fitted_devs_mean(fitted);
  std::for_each(fitted_devs_mean.begin(),
                fitted_devs_mean.end(),
                [mu](double &y) {
                  y = square(y - mu);
                });
  sse = sum(fitted_devs_mean);

  // R^2
  return sse / sst;
}

double rmse(const vec &y, const vec &fitted)
{
  // Computes the RMSE given a vector of outcomes and fitted values

  // Check if dimensions of inputs match
  if (y.size() != fitted.size())
  {
    std::cerr << "Outcome vector and fitted values need to have the same number of rows!";
    std::exit(2);
  }

  // Compute squared residuals
  vec residuals_sq{};
  residuals_sq.resize(y.size());

  for (int i{0}; i < y.size(); i++)
  {
    residuals_sq[i] = square(y[i] - fitted[i]);
  }

  // Root-mean-squared error
  return std::pow(mean(residuals_sq), 0.5);
}

vec rmse(const vec &y, const matrix &fitted)
{
  // Computes the RMSE given a vector of outcomes and a sequence of vectors containing fitted values

  // Check if dimensions of inputs match
  if (y.size() != nrow(fitted))
  {
    std::cerr << "Outcome vector and fitted values need to have the same number of rows!";
    std::exit(2);
  }

  vec rmses{};
  rmses.resize(ncol(fitted));

  for (int j{0}; j < ncol(fitted); j++)
  {
    rmses[j] = rmse(y, fitted[j]);
  }

  return rmses;
}

double mae(const vec &y, const vec &fitted)
{
  // Computes the MAE given a vector of outcomes and fitted values

  // Check if dimensions of inputs match
  if (y.size() != fitted.size())
  {
    std::cerr << "Outcome vector and fitted values need to have the same number of rows!";
    std::exit(2);
  }

  // Compute absolute residuals
  vec residuals_abs{};
  residuals_abs.resize(y.size());

  for (int i{0}; i < y.size(); i++)
  {
    residuals_abs[i] = std::abs(y[i] - fitted[i]);
  }

  // Mean absolute error
  return mean(residuals_abs);
}

void print_statistics(const vec &y, const vec &fitted)
{
  // Prints in-sample fit statistics given a vector of outcomes and fitted values
  std::cout << "\nIn-sample fit of optimal model:\n";
  std::cout << "R^2: " << r_squared(y, fitted) << "\n";
  std::cout << "RMSE: " << rmse(y, fitted) << "\n";
  std::cout << "MAE: " << mae(y, fitted) << "\n\n";
}