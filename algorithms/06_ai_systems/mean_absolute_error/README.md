# Mean Absolute Error (MAE)

## Motivation

Mean absolute error evaluates a regression model by measuring the **average magnitude of its prediction errors** in the same units as the target. Unlike squared error, MAE does not disproportionately square large residuals, although large deviations still raise the score.

## Formula and example

For aligned true targets `y` and predicted values `p` with `n>=1`:

```text
MAE(y, p) = (1/n) * sum(i=1..n) |y[i] - p[i]|
y = [2, -1, 5]; p = [1, 1, 3]
absolute errors = [1, 2, 2]
MAE = 5/3
```

A score of **0** is perfect; larger nonnegative scores indicate greater error. For a single observation, MAE is its absolute error.

## Shared contract

- Require **nonempty**, equal-length arrays of **finite real** values. Neither NaN, infinity, incompatible shapes nor overflowing subtractions are accepted.
- Use double precision. Accumulate `abs(y[i]-p[i])/n` to avoid avoidable overflow in the unnormalized sum; reject a non-finite final value. Tiny rounding differences can occur across languages.
- C accepts `const double *truth`, `const double *predictions`, `size_t n` and `double *out`. Null pointers and `n=0` are invalid. Output is unchanged on error; arrays and output must not alias.
- Python rejects boolean and nonnumeric entries. R requires real numeric vectors. Both raise errors for invalid inputs; Rust returns `Result<f64, &'static str>`.
- The score is **unweighted** and assumes aligned observations.

## Complexity

**O(n)** time. **O(1)** auxiliary memory in C, Python and Rust; vectorized R uses **O(n)** temporary space.

## Implementations

| Language | Directory |
| --- | --- |
| C | [mean_absolute_error_c](mean_absolute_error_c/) |
| Python | [mean_absolute_error_python](mean_absolute_error_python/) |
| Rust | [mean_absolute_error_rust](mean_absolute_error_rust/) |
| R | [mean_absolute_error_r](mean_absolute_error_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include perfect fits, signed residuals, one observation, large finite values, invalid coordinates, shape mismatches and subtraction overflow.

## Limitations

MAE does not distinguish underprediction from overprediction or capture prediction uncertainty. Compare it with other regression metrics, residual plots and application-specific costs. No sample weights, missing-value imputation or CUDA implementation are provided.

## References

Scikit-learn developers. (n.d.). *sklearn.metrics.mean_absolute_error*. Scikit-learn documentation. https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_absolute_error.html
