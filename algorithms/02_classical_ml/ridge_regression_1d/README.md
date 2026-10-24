# Univariate Ridge Regression

## Why it belongs in AI

**Ridge regression** extends ordinary least squares with an L2 penalty on
the model coefficient, reducing its magnitude when data are noisy or
collinear. This small, closed-form implementation makes the role of
regularization transparent without a linear-algebra package. It
complements ordinary linear regression and gradient descent already in
the catalogue (Hoerl & Kennard, 1970).

## Mathematical definition

Given paired observations `(x_i, y_i)`, fit the model
`prediction(x)=intercept+slope*x` by minimizing

```text
sum_i (y_i - intercept - slope*x_i)^2 + lambda*slope^2
```

The **intercept is not penalized**. Let `mx=mean(x)`,
`my=mean(y)`, `Sxx=sum_i (x_i-mx)^2`, and
`Sxy=sum_i (x_i-mx)(y_i-my)`.

```text
slope = Sxy / (Sxx + lambda)
intercept = my - slope*mx
```

If `Sxx=0` and `lambda=0`, every feature is identical and the slope
is unidentifiable. Choose **slope=0** and **intercept=my** explicitly.
This is the same fitted prediction on observed feature values as any
least-squares solution to the singular problem.

Example:

```text
x=[0,1,2], y=[1,3,5], Sxx=2, Sxy=4

lambda=0: slope=2, intercept=1
lambda=2: slope=1, intercept=2
predict(x=3, lambda=2) = 5
```

## Shared contract

- Exactly aligned nonempty real feature and target vectors, with
  **1 <= n <= 4096** and all entries finite and in **[-1e6,1e6]**.
  These pedagogical bounds keep centered sums representable in double.
- Nonnegative finite L2 penalty `lambda` in **[0,1e12]**.
  The objective uses a **sum of squared residuals**, not a mean, so
  the regularization parameter scales with sample count.
- Return the finite pair `(intercept,slope)`; no output predictions
  are computed by this function.
- Validate all input, including alignment, finite arithmetic,
  centered sums, denominator, fitted parameters, and penalty. Reject
  NaN/Inf, malformed numeric values, mismatches and invalid penalties.
- C receives `double x[n], y[n]`, scalar `lambda`, and two
  caller-owned disjoint `double*` outputs. Both are unchanged on
  failure, and inputs/outputs must not overlap. Rust returns
  `Result<(f64,f64),...>`; Python returns a pair; R returns a
  named list. Python rejects boolean coordinates.
- For a single training point, the slope is 0 and intercept equals
  its target, for any allowed lambda.

## Complexity

O(n) time, **O(1)** auxiliary memory. Feature centering helps avoid
large raw `sum(x²)-n*mean(x)²` cancellation, although floating-point
accuracy remains finite.

## Implementation and tests

| Language | Location |
| --- | --- |
| C | [ridge_regression_1d_c](ridge_regression_1d_c/) |
| Python | [ridge_regression_1d_python](ridge_regression_1d_python/) |
| Rust | [ridge_regression_1d_rust](ridge_regression_1d_rust/) |
| R | [ridge_regression_1d_r](ridge_regression_1d_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover unregularized and penalized fits, a constant-feature case,
a singleton, negative penalties, invalid dimensions and nonfinite data.

## Limitations

One feature and squared-error regression only. No feature pipeline,
multidimensional matrix inverse, standard-error estimates,
cross-validation, intercept regularization, Lasso, stochastic training
or GPU implementation. This is an educational closed-form baseline.

## Reference

Hoerl, A. E., & Kennard, R. W. (1970). Ridge regression: Biased
estimation for nonorthogonal problems. *Technometrics, 12*(1), 55–67.
https://doi.org/10.1080/00401706.1970.10488634
