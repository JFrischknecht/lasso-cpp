// A minimal working implementation of the LASSO estimator for linear models with C++
// Author: Joel Frischknecht

// Include custom header files
#include "constants.h"
#include "timing.h"
#include "matrix.h"
#include "import.h"
#include "stats.h"
#include "algorithm.h"
#include "fit.h"
#include "export.h"

// Include standard libraries
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib> // for std::exit()

int main(int argc, char *argv[])
{
  std::cout << "Welcome to this simple LASSO implementation in C++"
            << "\n";

  // Start timer
  Timer t;

  // Initialize file names with default values
  std::string outcome_file{defaults::outcome_input_file_name};
  std::string predictors_file{defaults::predictors_input_file_name};

  // Read optional command line parameters to overwrite default file names
  if (argc != 3 & argc != 1)
  {
    std::cerr << "Please follow this format. lasso.exe [outcome.csv] [predictors.csv]";
    std::exit(2);
  }
  else if (argc == 3)
  {
    outcome_file = argv[1];
    predictors_file = argv[2];
  }

  // Import data
  std::cout << "\nImporting data:"
            << "\n";
  matrix outcome{read_csv(outcome_file)};
  matrix predictors{read_csv(predictors_file)};

  if (nrow(outcome) != nrow(predictors))
  {
    std::cerr << "Outcome vector and predictor matrix need to have the same number of rows!";
    std::exit(2);
  }

  // Standardize predictors
  matrix predictors_s{standardize(predictors)};

  // Center outcome
  matrix outcome_c{center(outcome)};

  // Build a data-adaptive grid of penalty parameters (lambdas) 
  vec lambdas{create_lambda_grid(predictors_s, outcome_c)};

  // For each lambda value in the grid: Apply 5-fold cross-validation to estimate the cross-validated RMSE of each lasso model (one model for each lambda value)
  std::cout << "\n5-fold Cross-Validation:\n";
  std::cout << "Folds are constructed with a time-based random seed, "
            << "meaning that results may differ slightly with every execution."
            << "\n";
  std::cout << "Computing...\n";

  vec cv_rmses{lasso_cv(predictors, outcome, lambdas, 5)};

  // Find the index of the RMSE-minimal model, as well as the associated RMSE
  int index_minimal{which_min(cv_rmses)};
  double cv_rmse_minimal{*std::min_element(cv_rmses.begin(), cv_rmses.end())};

  // Find the lambda that minimizes the RMSE
  double lambda_optimal{lambdas[index_minimal]};
  
  std::cout << "\nMinimal cross-validated RMSE of " << cv_rmse_minimal
            << " reached with a penalty parameter of " 
            << lambda_optimal << ".\n";

  // Re-estimate lasso with optimal penalty parameter on the full dataset
  std::cout << "Re-estimating lasso model with optimal penalty parameter "
            << "on the full dataset.\n";

  vec beta_null(ncol(predictors_s), 0.0);
  vec beta_optimal{coordinate_descent_lasso(predictors_s, outcome_c,
                                            beta_null, lambda_optimal)};

  // Recover the estimated intercept of the optimal model
  double intercept_optimal{recover_intercept(beta_optimal,
                                             mean(predictors),
                                             mean(matrix_to_vector(outcome)))};

  // Predict fitted values on for the optimal model
  vec fitted_values_optimal{predict(predictors_s,
                                    intercept_optimal,
                                    beta_optimal)};

  // Print in-sample fit statistics of optimal lasso model
  print_statistics(matrix_to_vector(outcome), fitted_values_optimal);

  // Export predictions of optimal lasso
  write_csv(defaults::outcome_output_file_name,
            fitted_values_optimal, defaults::outcome_header);
  std::cout << "Fitted values of optimal model "
            << "successfully exported to "
            << defaults::outcome_output_file_name << "!\n";

  // Export estimated coefficients including the intercept
  write_csv(defaults::coefs_output_file_name,
            add_intercept(beta_optimal, intercept_optimal), defaults::coefficient_header);
  std::cout << "Coefficients of optimal model, including the intercept as "
            << "first entry, successfully exported to "
            << defaults::coefs_output_file_name << "!\n";

  // Display runtime to user
  std::cout << "\nTotal runtime: " << t.elapsed() << " seconds";
  std::cout << "\nEnd of program. ";

  return 0;
}