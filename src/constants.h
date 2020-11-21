// Header file to hold global constants

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace defaults
{
  const std::string outcome_input_file_name{"outcome.csv"};
  const std::string predictors_input_file_name{"predictors.csv"};

  const std::string outcome_output_file_name{"lasso_fitted.csv"};
  const std::string coefs_output_file_name{"lasso_coefficients.csv"};

  const std::string outcome_header{"lasso_fitted"};
  const std::string coefficient_header{"lasso_coefficients"};
} 

#endif