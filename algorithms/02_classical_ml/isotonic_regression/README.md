# Isotonic Regression (Pool-Adjacent-Violators)

## Why it belongs in this repository

Isotonic regression enforces a monotonic relationship while minimizing squared error. It is useful for monotonic probability calibration and fitting ordered targets without assuming a linear trend. The pool-adjacent-violators (PAV) algorithm gives the exact solution for equally weighted observations in their supplied order (Barlow et al., 1972).

## What it does

For ordered observations y[1], ..., y[n], find fitted z that minimize

    sum((z[i] - y[i])^2, i = 1..n)

subject to z[1] <= z[2] <= ... <= z[n]. Start with each observation as a unit-weight block; whenever adjacent block means are decreasing, pool those blocks and replace their values by the weighted mean. The final block means are nondecreasing.

## Contract

- Input: a one-dimensional ordered sequence of 1 to 512 finite real observations, each in [-1,000,000, 1,000,000].
- Output: a new same-length nondecreasing sequence minimizing squared error; equal weights, no sorting and no mutation required.
- C: Boolean success; invalid pointer, length or value returns false without modifying the output buffer. In-place output is allowed.
- Python: list or tuple of numeric values (bool rejected), raising ValueError on invalid input.
- Rust: f64 slice returning None on invalid input, Some(vector) otherwise.
- R: finite one-dimensional numeric vector (not logical or matrix); invalid input raises an error.
- Intermediate sums are bounded under the stated input limits; floating-point rounding may differ slightly among languages.

## Complexity

O(n) time and O(n) auxiliary space for the block stack and fitted output.

## Implementations

| Language | Folder |
| --- | --- |
| C | [isotonic_regression_c](isotonic_regression_c/) |
| Python | [isotonic_regression_python](isotonic_regression_python/) |
| Rust | [isotonic_regression_rust](isotonic_regression_rust/) |
| R | [isotonic_regression_r](isotonic_regression_r/) |

## Examples and tests

    [3, 1, 2, 4] -> [2, 2, 2, 4]
    [3, 2, 1]    -> [2, 2, 2]
    [-2, 2, 0, 4] -> [-2, 1, 1, 4]

Tests include increasing, decreasing, tied, singleton, 512-observation and invalid-value cases. From the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is equally weighted univariate isotonic regression on a fixed input order, not a general partially ordered or multivariate fit. It does not perform out-of-sample interpolation, confidence intervals or automatic ordering. Input bounds are deliberately conservative for a small, portable reference implementation.

## References

Barlow, R. E., Bartholomew, D. J., Bremner, J. M., & Brunk, H. D. (1972). *Statistical inference under order restrictions: The theory and application of isotonic regression*. Wiley.
