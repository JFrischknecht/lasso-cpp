// General statistical functions

#include "matrix.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <iostream>
#include <cstdlib> // for std::exit()
#include <random>  // for std::random_device
#include <chrono>  // for std::chrono::system_clock

double square(const double &x)
{
  // Squares a number
  return std::pow(x, 2);
}

double sum(const vec &x)
{
  // Computes the sum of the elements of a vector
  return std::accumulate(x.begin(), x.end(), 0.0, std::plus{});
}

double mean(const vec &x)
{
  // Computes the mean of a vector
  return sum(x) / x.size();
}

vec mean(const matrix &x)
{
  // Compute column means of a matrix
  vec means{};

  for (vec column : x)
  {
    means.push_back(mean(column));
  }

  return means;
}

vec rowmean(const matrix &x)
{
  // Compute row means of a matrix
  vec means(nrow(x), 0.0);

  for (int i{0}; i < nrow(x); i++)
  {

    // Build the row-sum
    for (int j{0}; j < ncol(x); j++)
    {
      means[i] += x[j][i];
    }

    // Divide by the number of columns
    means[i] = means[i] / ncol(x);
  }

  return means;
}

double sd(const vec &x)
{
  // Computes the standard-deviation of a vector, using N instead of N-1 in
  // the denominator
  double var{};
  double mu{mean(x)};
  vec squared_devs{};
  squared_devs.resize(x.size());

  // Compute squared deviations
  for (int i{0}; i < x.size(); i++)
  {
    squared_devs[i] = square(x[i] - mu);
  }

  // Note the use of "N" and not "N-1" in the denominator here
  var = sum(squared_devs) / x.size();

  return std::pow(var, 0.5);
}

vec sd(const matrix &x)
{
  // Compute the column standard-deviations of a matrix
  vec sds{};

  for (vec column : x)
  {
    sds.push_back(sd(column));
  }

  return sds;
}

double inner_product(const vec &x, const vec &y)
{
  // Check if dimensions of inputs match
  if (x.size() != y.size())
  {
    std::cerr << "Vectors need to have the same size!";
    std::exit(2);
  }

  // Compute the inner product between two vectors
  double inner_product{0};

  for (int i{0}; i < x.size(); i++)
  {
    inner_product += (x[i] * y[i]);
  }

  return inner_product;
}

vec center(const vec &x)
{
  // Centers a vector by subtracting the mean from every element
  vec centered{};
  centered.resize(x.size());

  double mu = {mean(x)};

  for (int i{0}; i < x.size(); i++)
  {
    centered[i] = x[i] - mu;
  }

  return centered;
}

vec standardize(const vec &x)
{
  // Standardizes a vector by centering it and by dividing every element by the standard-deviation
  vec centered{center(x)};
  vec standardized{};
  standardized.resize(x.size());

  double sigma{sd(x)};

  for (int i{0}; i < x.size(); i++)
  {
    standardized[i] = centered[i] / sigma;
  }

  return standardized;
}

matrix center(const matrix &x)
{
  // Centers every column of a matrix
  matrix centered{allocate_matrix(ncol(x), nrow(x))};

  for (int j{0}; j < ncol(x); j++)
  {
    centered[j] = center(x[j]);
  }
  return centered;
}

matrix standardize(const matrix &x)
{
  // Standardizes every column of a matrix
  matrix standardized{allocate_matrix(ncol(x), nrow(x))};

  for (int j{0}; j < ncol(x); j++)
  {
    standardized[j] = standardize(x[j]);
  }
  return standardized;
}

vec sequence(const double start, const double end, const int length)
{
  // Creates a sequence starting with 'start' ending with 'end' with a total of 'length' values that are evenly spaced between 'start' and 'end'
  vec seq{};

  double step_size{(end - start) / (length - 1)};

  // Initialise value
  seq.push_back(start);

  for (int i{1}; i < length; i++)
  {
    seq.push_back(seq[i - 1] + step_size);
  }

  // Override ending value
  seq[length - 1] = end;

  return seq;
}

vec_int sequence(const int start, const int length)
{
  // Creates an integer sequence: start, start+1, ...., start + (length-1)
  vec_int seq{};

  for (int i{0}; i < length; i++)
  {
    seq.push_back(start + i);
  }

  return seq;
}

int which_min(const vec &x)
{
  // Determines which index of a vector is the minimal element
  int index_min{0};
  double value{x[0]};

  for (int i{1}; i < x.size(); i++)
  {
    if (x[i] < value)
    {
      value = x[i];
      index_min = i;
    }
  }

  return index_min;
}

vec subset(const vec &x, const vec_int &indices)
{
  // Returns a vector containing only the elements with matching indices
  vec sub{};

  for (auto i : indices)
  {
    sub.push_back(x[i]);
  }

  return sub;
}

matrix subset(const matrix &x, const vec_int &indices)
{
  // Returns a matrix containing only the rows matching the supplied indices
  matrix sub{};
  sub.resize(ncol(x));

  for (int j{0}; j < ncol(x); j++)
  {
    sub[j] = subset(x[j], indices);
  }

  return sub;
}

matrix_int create_folds(const int length, const int n_folds)
{
  // Splits a sequence 0, 1, ..., (length-1) into equally sized folds,
  // discarding any potential remainder and returning a matrix of indices, each
  // column-vector containing the indices of a single fold
  vec_int seq{sequence(0, length)};

  // Randomly shuffle the sequence
  auto seed{std::chrono::system_clock::now().time_since_epoch().count()};
  std::shuffle(seq.begin(), seq.end(), std::default_random_engine(seed));

  // Compute fold-size: Use integer division, to automatically lose fractional part (same as rounding down)
  int fold_size{length / n_folds};

  // Container
  matrix_int folds{allocate_matrix_int(n_folds, fold_size)};

  // Construct each fold as a column in the matrix
  // Any potential remainder is not considered!
  for (int j{0}; j < n_folds; j++)
  {
    for (int i{0}; i < fold_size; i++)
    {
      folds[j][i] = seq[i + (j * fold_size)];
    }
  }

  return folds;
}

matrix create_test_set(const matrix &x, const matrix_int &folds, const int active_fold)
{
  // Given a data matrix, a matrix containing the folds and information on the active fold, produce the test set, i.e. subset the data matrix with the row-indices of the active fold
  return subset(x, folds[active_fold]);
}

matrix create_training_set(const matrix &x, const matrix_int &folds, const int active_fold)
{
  // Given a data matrix, a matrix containing the folds and information on the active fold, produce the training set, i.e. subset the data matrix with the row-indices of all but the active fold
  vec_int remaining_indices{};

  for (int j{0}; j < ncol(folds); j++)
  {
    if (j != active_fold)
    {
      for (int i{0}; i < nrow(folds); i++)
      {
        remaining_indices.push_back(folds[j][i]);
      }
    }
  }

  return subset(x, remaining_indices);
}
