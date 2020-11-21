# lasso_cpp
A minimal working implementation of the LASSO estimator for linear models with C++

## About



## How to use the program 

This program requires two inputs. First it requires a CSV-file containing the continuous/binary outcome variable. Second it requires a CSV-file containing the continuous/binary predictors. The first row of each file (where column-names are usually placed) is not read. Both files must consist of only numeric columns without missing values. The user is responsible to clean and prepare the data first (e.g. deal with any missing values, one-hot-encode categorical predictors, etc.). Note that and intercept should not be included in the set of predictors (i.e. the file with the predictors should not contain a constant column).


Execute lasso.exe in any command-line. By default the program will look for files named

## References

Friedman, J., Hastie, T., & Tibshirani, R. (2010). Regularization Paths for Generalized Linear Models via Coordinate Descent. Journal of Statistical Software, 33(1), 1 - 22. http://dx.doi.org/10.18637/jss.v033.i01

Hastie, T., Tibshirani, R., & Wainwright, M. (2015). Statistical Learning with Sparsity: The Lasso and Generalizations. Chapman & Hall/CRC.
