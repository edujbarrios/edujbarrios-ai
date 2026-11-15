# Split Conformal Prediction Intervals for Regression

## Why it belongs in this repository

Split conformal prediction adds a **calibrated uncertainty interval** to a
fixed regression model without requiring assumptions about its functional
form. It makes finite-sample uncertainty quantification accessible for
model evaluation and deployment (Angelopoulos & Bates, 2023).

## Algorithm

Given `n` held-out targets `y[i]`, their fixed-model predictions
`p[i]`, a requested miscoverage `alpha` in (0, 1), and new predictions:

1. Compute nonconformity residuals `r[i] = abs(y[i] - p[i])`.
2. Sort residuals increasingly and set
   `k = ceil((n + 1) * (1 - alpha))` (a **one-based** rank).
3. If `k <= n`, choose radius `q = sorted(r)[k-1]`.
   Otherwise choose `q = +infinity`.
4. For each new regression prediction `p_new`, return
   `[p_new - q, p_new + q]`.

The `(n + 1)` factor is essential: replacing it with an ordinary sample
percentile can invalidate the finite-sample guarantee. Under exchangeability
of calibration and test examples, and a predictor fitted without the
calibration labels, the **marginal** coverage is at least `1-alpha`.
It is not guaranteed conditional on particular features.

## Contract

- Python accepts lists or tuples of calibration targets, aligned predictions,
  and zero or more test predictions. Calibration arrays must be nonempty and
  equally long (maximum 100,000 entries per input vector).
- Numeric values must be real, finite, and non-boolean. `alpha` must be
  finite with `0 < alpha < 1`; invalid inputs raise `ValueError`.
- Output is one `(lower, upper)` tuple per test prediction. Inputs are
  never modified.
- If `k > n`, return `(-inf, inf)`: this is valid but uninformative,
  especially for small calibration sets and low miscoverage levels.
- Overflowing residuals or finite interval endpoints cause `ValueError`.
  Computations use Python binary64 and normal floating-point rounding.

## Worked example

For calibration targets `[1, 2, 3, 4]` and model predictions
`[0, 2, 1, 4]`, the sorted absolute residuals are `[0, 0, 1, 2]`.
With `alpha=0.25`, `k=ceil(5 * 0.75)=4`, so `q=2`.
Test predictions `[0, 10]` give `[(-2, 2), (8, 12)]`.

## Complexity

`O(n log n + m)` time for sorting `n` calibration residuals and
constructing `m` intervals; `O(n + m)` auxiliary memory.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [split_conformal_regression_python](split_conformal_regression_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover rank selection, zero-width intervals, empty test sets, small
calibration sets, input validation, overflow, and input immutability.

## Limitations

The model must be fixed relative to the calibration set. The guarantee relies
on exchangeability and may fail under distribution shift or data leakage.
Intervals are symmetric and have constant width; they do not adapt to
heteroscedasticity. They can be infinite for small `n`. No conditional
coverage or group-fairness guarantee is implied.

## References

Angelopoulos, A. N., & Bates, S. (2023). Conformal prediction: A gentle
introduction. *Foundations and Trends in Machine Learning, 16*(4), 494–591.
https://doi.org/10.1561/2200000101
