# Weighted Mean Absolute Error

## Why it belongs here

Weighted mean absolute error (WMAE) evaluates regression forecasts while
allowing each example to contribute differently. This is useful for
importance-weighted evaluation and datasets where some observations carry
higher relevance. It extends ordinary MAE with nonnegative sample weights
(scikit-learn developers, n.d.).

## Algorithm

For targets `y[i]`, predictions `p[i]` and weights `w[i]`:

```text
WMAE = sum_i(w[i] * abs(y[i] - p[i])) / sum_i(w[i])
```

For `y=[1,3]`, `p=[2,1]`, `w=[1,3]`, the result is
`(1*1 + 3*2)/4 = 1.75`. A zero weight excludes its example.

## Contract

- All three vectors must have the same length `1..4096`.
- Targets/predictions must be finite real numbers in `[-1e6,1e6]`.
  Weights must be finite in `[0,1e6]` with a **strictly positive**
  total weight.
- Output: a finite nonnegative scalar measured in the same units as the
  targets. Zero-weight entries are allowed, but all-zero weights are invalid.
- Empty, mismatched, nonfinite or out-of-range input is rejected. Python
  rejects bool entries and nonnumeric values. R requires real numeric
  (not logical/complex) vectors.
- C writes a caller-owned scalar only on success; Rust returns
  `Result<f64,&str>`; Python/R raise exceptions.
- Bounds ensure finite sums in these reference implementations without
  extra numerical libraries; they are deliberate educational limits.

## Complexity

Time **O(n)**; auxiliary memory **O(1)** beyond input/output storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [weighted_mean_absolute_error_c](weighted_mean_absolute_error_c/) |
| Python | [weighted_mean_absolute_error_python](weighted_mean_absolute_error_python/) |
| Rust | [weighted_mean_absolute_error_rust](weighted_mean_absolute_error_rust/) |
| R | [weighted_mean_absolute_error_r](weighted_mean_absolute_error_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Test cases include unequal example weights, zero-weight examples, perfect
predictions, singleton input, all-zero weights and invalid lengths, values
and types.

## Limitations

Weighted MAE is not a differentiable training loss at zero residual, does
not normalize weights independently before computing the ratio and does
not estimate uncertainty. For severely skewed weights the effective sample
size may be much smaller than the nominal sample size.

## References

scikit-learn developers. (n.d.). *mean_absolute_error*.
https://scikit-learn.org/stable/modules/generated/sklearn.metrics.mean_absolute_error.html
