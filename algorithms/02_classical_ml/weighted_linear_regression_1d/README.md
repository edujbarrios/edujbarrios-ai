# Weighted Linear Regression (One Feature)

## Why it belongs here

Weighted least squares generalizes linear regression by assigning
different importance to observations. It is useful when sample
reliability varies or repeated observations are summarized by
weights. This one-feature reference uses the centered,
closed-form solution (Hastie et al., 2009).

## Mathematical definition

Given aligned feature values `x[i]`, targets `y[i]`,
and nonnegative observation weights `w[i]`, fit

```text
y_hat = intercept + slope * x
```

by minimizing `sum(w[i] * (y[i] - y_hat[i])^2)`.
With `W = sum(w)`, define weighted means

```text
mx = sum(w[i] * x[i]) / W
my = sum(w[i] * y[i]) / W
denom = sum(w[i] * (x[i] - mx)^2)
numer = sum(w[i] * (x[i] - mx) * (y[i] - my))
slope = numer / denom
intercept = my - slope * mx
```

The model requires `W > 0` and `denom > 0`. Zero-weight
observations are ignored in the fit, but still validated.
For `x=[0,1,2], y=[1,2,2], w=[1,1,2]`, the result is
`slope=5/11` and `intercept=13/11`. With `x=[0,1,2]`,
`y=[1,3,5]`, and equal weights, the fit is `slope=2`,
`intercept=1`.

## Contract

- Three equal-length aligned vectors, `2 <= n <= 128`.
- Feature values and targets must be finite in
  `[-1,000,000,1,000,000]`.
- Weights must be finite in `[0,1,000,000]`. At least one
  positive weight and positive weighted feature variance required.
- Returns finite double-precision `slope` and `intercept`.
- C takes three double arrays, `n` and distinct caller-owned
  output pointers. It returns false on any invalid input
  and leaves **both outputs unchanged**. Neither output can
  overlap inputs or each other.
- Rust returns `Result<(f64,f64), &str>`. Python takes lists/tuples
  of numeric values (rejecting booleans); it returns a tuple.
  R takes numeric vectors, not matrices, returning a named
  numeric vector `c(slope=..., intercept=...)`.

## Complexity

**O(n)** time, **O(1)** extra working memory in C/Rust.
Python and R create validated/converted vectors of size **O(n)**.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [weighted_linear_regression_1d_c](weighted_linear_regression_1d_c/) |
| Python | [weighted_linear_regression_1d_python](weighted_linear_regression_1d_python/) |
| Rust | [weighted_linear_regression_1d_rust](weighted_linear_regression_1d_rust/) |
| R | [weighted_linear_regression_1d_r](weighted_linear_regression_1d_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover weighted and unweighted reference fits, zero weights,
degenerate design matrices, zero total weight, nonfinite values,
invalid shapes, negative and oversized weights.

## Limitations

This is a one-feature analytic fit, not a multivariate matrix
solver. It does not report confidence intervals, residuals,
regularization or a prediction interface. For poorly scaled
features, floating-point rounding can make the centered variance
small or zero; production systems should consider conditioning.

## References

- Hastie, T., Tibshirani, R., & Friedman, J. (2009).
  *The elements of statistical learning: Data mining, inference,
  and prediction* (2nd ed.). Springer.
  https://hastie.su.domains/ElemStatLearn/
