# Exponential Moving Average (EMA)

## Why it matters

An exponential moving average (EMA) smooths noisy measurements and tracks a changing level using exponentially decaying weights. EMA appears in time-series forecasting, monitoring and optimization (including momentum-like updates). This entry models **one-dimensional sequence smoothing**, not model parameters or a full forecaster.

## Recurrence and example

Given observations `x[0..n-1]` and smoothing coefficient `0 < alpha <= 1`:

```text
ema[0] = x[0]
ema[t] = (1 - alpha) * ema[t-1] + alpha * x[t]   for t >= 1

x = [10, 20, 10], alpha = 0.5
ema = [10, 15, 12.5]
```

A smaller `alpha` smooths more strongly; `alpha=1` returns the original observations. Computing the weighted terms separately avoids the overflow of `x[t] - ema[t-1]` for large, opposite-signed values.

## Shared contract

- Require at least **one** finite numeric observation and a finite `alpha` in the interval **(0, 1]**.
- Return an output sequence with exactly the same number of elements. The first value equals the first observation; subsequent values follow the recurrence in double precision.
- Non-finite intermediate outputs are errors. No clipping or silent replacement of invalid observations.
- C takes `const double *values`, `size_t n`, `double alpha` and caller-owned `double *out` (at least `n` elements). Input and output must not overlap; it validates a full dry run before writing, leaving output unchanged on error.
- Rust returns `Result<Vec<f64>, &'static str>`. Python accepts a list or tuple of numeric values but rejects booleans. R accepts finite real numeric vectors. Invalid input raises an error in Python/R.

## Complexity

**O(n)** time and **O(n)** space for the returned series (or caller-provided output array in C); **O(1)** working memory beyond the output.

## Implementations

| Language | Directory |
| --- | --- |
| C | [exponential_moving_average_c](exponential_moving_average_c/) |
| Python | [exponential_moving_average_python](exponential_moving_average_python/) |
| Rust | [exponential_moving_average_rust](exponential_moving_average_rust/) |
| R | [exponential_moving_average_r](exponential_moving_average_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the recurrence, constant sequences, `alpha=1`, very large opposite-signed inputs, empty sequences, non-finite observations and invalid coefficients.

## Limitations

The initialization `ema[0]=x[0]` gives the first observations more influence than a stationary process might suggest. This does not estimate `alpha`, missing observations, trends or seasonality. Different initialization schemes yield different values.

## References

Hyndman, R. J., & Athanasopoulos, G. (2021). *Forecasting: Principles and practice* (3rd ed.). OTexts. https://otexts.com/fpp3/ses.html
