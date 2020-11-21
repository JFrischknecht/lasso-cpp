# lasso_cpp

I provide a minimal working implementation of the popular LASSO estimator for linear models with the programming language C++. 


## About

The LASSO estimator is implemented with the well-known cyclic coordinate descent algorithm introduced by Friedman, Hastie and Tibshirani (2010) and described in detail in Hastie, Tibshirani, and Wainwright (2015, pp. 7-17). A data adaptive grid of penalty parameters is built following the suggestions of Friedman, Hastie and Tibshirani (2010, p. 7). The main algorithm uses "warm-starts" and active set iteration to speed up the computations (see Friedman, Hastie and Tibshirani (2010, p. 7) and Hastie, Tibshirani, and Wainwright (2015, p. 17)).

The optimal penalty parameter is picked via 5-fold cross-validation. Optimal in this context means minimizing the cross-validated RMSE (Root Mean Square Error). The program then re-estimates the LASSO model using the optimal penalty parameter, prints in-sample fit statistics to the console and exports the fitted values aswell as the estimated coefficients.


## How to use the program 

This program requires two inputs. First it requires a CSV-file containing the continuous/binary outcome variable. Second it requires a CSV-file containing the continuous/binary predictor(s). The first row of each file (where column-names are usually placed) is not read. No thousands separator should be present in the CSV-files. A point should be used as decimal separator. Both files must consist of only numeric columns without missing values. None of the data should be standardized or centered beforehand. The user is responsible to clean and prepare the data first (e.g. deal with any missing values, one-hot-encode categorical predictors, etc.). Note that an intercept should not be included in the set of predictors.

By default the name of the first file is assumed to be *outcome.csv* and the name of the second file is assumed to be *predictors.csv*.

To run the program:
1. Download *lasso.exe*, *outcome.csv*, *predictors.csv* and save them together in a folder.
2. Start up a command-line (e.g. *cmd* on Windows) in that folder and execute *lasso.exe* in the command line. In the Windows command line, e.g.
```shell
D:\lasso_cpp-main>lasso.exe
```
You can use data from differently named files by supplying two optional command-line arguments. In that case you always need to supply the name of both files. In the Windows command line, e.g.
```shell
D:\lasso_cpp-main>lasso.exe outcome_large.csv predictors_large.csv
```

The program will produce two files, *lasso_fitted.csv* and *lasso_coefficients.csv*, in the folder where the program was executed.
The first file contains the fitted values of the optimal model and the second contains the estimated coefficients of the optimal model, with the intercept being the first coefficient.

The program was tested on three different Windows 10 machines.


## Data to test the program

*outcome.csv* and *predictors.csv* are small test datasets provided by the R package [glmnet](https://github.com/cran/glmnet).

*outcome_large.csv* and *predictors_large.csv* contain simulated data spanning one million rows. The outcome is a standard normally distributed random variable. The set of predictors features an independently drawn Bernoulli random variable (*p = 0.5*) and an independently drawn standard normal random variable. Note that the outcome and the predictors are stochastically independent by construction.

## References

Friedman, J., Hastie, T., & Tibshirani, R. (2010). Regularization Paths for Generalized Linear Models via Coordinate Descent. Journal of Statistical Software, 33(1), 1 - 22. http://dx.doi.org/10.18637/jss.v033.i01

Hastie, T., Tibshirani, R., & Wainwright, M. (2015). Statistical Learning with Sparsity: The Lasso and Generalizations. Chapman & Hall/CRC.
