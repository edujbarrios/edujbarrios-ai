# Symmetric Mean Absolute Percentage Error (sMAPE)

## Why it belongs here

Evaluation of forecasting and regression models sometimes requires a
relative error rather than an error measured in the original target
units. **Symmetric mean absolute percentage error (sMAPE)** compares
the absolute prediction error with the magnitudes of *both* values.
Its definition and interpretation require care, particularly near
zero (Hyndman & Koehler, 2006).

## Mathematical definition

For targets `t[i]` and predictions `p[i]`:

```text
term[i] = 0                       if t[i] == 0 and p[i] == 0
          2 * abs(p[i] - t[i]) /
              (abs(p[i]) + abs(t[i]))   otherwise
sMAPE = mean(term[i])
```

The returned result is a **ratio in [0,2]**, not a percentage;
multiply it by 100 for the usual 0–200% presentation.
The all-zero pair convention is explicitly set to 0.

Example: targets `[0,1,-1]` and predictions `[0,2,1]`
produce terms `[0,2/3,2]`, so the mean is **8/9**.
Identical vectors return zero. Predictions exactly opposite
nonzero targets give the maximal score of 2.

## Contract

- Equal-length aligned target and prediction arrays of
  **1 to 10,000** finite real numbers.
- Each number must lie in `[-1,000,000,1,000,000]`.
  No NaNs, infinities, missing data or implicit coercions.
- Positive, negative and zero observations are supported.
  When both members of a pair are zero, that term is **zero**;
  when just one is zero, that term is 2.
- Output is a finite scalar in `[0,2]`, clamped only to
  correct negligible floating-point roundoff.
- C accepts separate input arrays and a caller-owned scalar output
  left unchanged on error; the output must not overlap inputs.
  Rust returns `Result<f64, &str>`. Python accepts lists/tuples
  and rejects boolean/nonnumeric entries; R accepts numeric
  vectors, not matrices.

## Complexity

**O(n)** time and **O(1)** auxiliary working memory.
Python and R also allocate small vectors for input conversion
or vectorized computation; this is **O(n)** auxiliary space.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [symmetric_mean_absolute_percentage_error_c](symmetric_mean_absolute_percentage_error_c/) |
| Python | [symmetric_mean_absolute_percentage_error_python](symmetric_mean_absolute_percentage_error_python/) |
| Rust | [symmetric_mean_absolute_percentage_error_rust](symmetric_mean_absolute_percentage_error_rust/) |
| R | [symmetric_mean_absolute_percentage_error_r](symmetric_mean_absolute_percentage_error_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover exact matches, zeros, opposite signs, mixed examples,
singletons, extreme permitted input values, nonfinite data,
mismatched/empty arrays and invalid types.

## Limitations

The metric can be unstable for values near zero and can be
misinterpreted as a conventional percentage error. It is
not suitable as a universal measure of predictive performance.
This reference supplies no weights, masking, confidence intervals
or batch-axis reduction.

## References

- Hyndman, R. J., & Koehler, A. B. (2006). Another look at measures
  of forecast accuracy. *International Journal of Forecasting,
  22*(4), 679–688.
  https://doi.org/10.1016/j.ijforecast.2006.03.001
