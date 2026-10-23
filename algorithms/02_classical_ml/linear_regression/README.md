# Simple Linear Regression (Ordinary Least Squares)

**Stage:** Classical Machine Learning · **Languages:** C, Python, Rust, R.

## Why it belongs in the repository

Linear regression is a fundamental supervised-learning baseline for mapping
an input feature to a numeric prediction. It teaches how a model's parameters
are fitted by minimizing squared residuals, and leads naturally to multiple
regression, gradient descent and regularization (Hastie et al., 2009).

## What it does

Fit a one-feature model `y_hat = intercept + slope * x` using ordinary
least squares. With `n` paired observations:

```text
x_mean = sum(x) / n
y_mean = sum(y) / n
slope = sum((x_i - x_mean)(y_i - y_mean)) / sum((x_i - x_mean)^2)
intercept = y_mean - slope * x_mean
```

For `x=[0,1,2,3]`, `y=[1,3,5,7]`, it produces `slope=2`,
`intercept=1`.

## Shared contract

- Inputs: **at least two paired observations**, with equal lengths, finite
  real-valued `x` and `y`, and nonzero variance in `x`.
- Output: finite `(slope, intercept)`, ordered consistently in each language.
- Reject constant input features, NaN, infinity, empty/mismatched lengths and
  unrepresentable intermediate values. No silent `NaN` outputs.
- C writes to distinct output pointers only on success, and requires valid
  buffers of at least `n` elements. Python/R raise errors; Rust returns
  `Result<(f64,f64), ...>`.
- The mean is updated incrementally, then centered sums are calculated in
  a second pass. This is more stable than subtracting raw sums of squares,
  but **not an arbitrary-precision fit**. Very large or near-degenerate
  inputs may be rejected or lose accuracy.
- Ordinary least squares is a mathematical objective, not a causal claim.

## Complexity

Two passes: **O(n)** time; **O(1)** auxiliary memory. No third-party
numerical packages or ML frameworks are needed.

## Implementations

| Language | Folder |
| --- | --- |
| C | [linear_regression_c](linear_regression_c/) |
| Python | [linear_regression_python](linear_regression_python/) |
| Rust | [linear_regression_rust](linear_regression_rust/) |
| R | [linear_regression_r](linear_regression_r/) |

There is no CUDA version in this PR; the one-dimensional reference fit does
not justify a GPU implementation without batching or credible benchmarking.

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The suites cover the exact `y=2x+1` fit, horizontal line, constant `x`,
small datasets, mismatched sizes, non-finite inputs and numerical overflow.

## Limitations

One predictor only; no multivariate features, weights, confidence intervals,
regularization, missing values or automatic differentiation. The objective
can be sensitive to outliers. This is educational/reference code, not a
production training system.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of
statistical learning: Data mining, inference, and prediction* (2nd ed.).
Springer. https://doi.org/10.1007/978-0-387-84858-7
