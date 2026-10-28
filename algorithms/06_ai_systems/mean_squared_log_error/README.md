# Mean Squared Logarithmic Error (MSLE)

## Why it belongs here

MSLE evaluates nonnegative regression predictions by comparing
their values on a logarithmic scale. It gives less influence to
large raw magnitudes than ordinary mean squared error and is
often discussed for nonnegative quantities with multiplicative
growth (scikit-learn developers, n.d.).

## Mathematical definition

For paired targets `t[i]` and predictions `p[i]`, compute

```text
MSLE = (1/n) * sum((log1p(p[i]) - log1p(t[i]))^2).
```

Using `log1p` maintains accuracy for small values and avoids
explicitly adding 1 to the inputs before taking a logarithm.
All target and prediction values must be nonnegative.

Examples: identical vectors have score zero. Targets `[0,1,9]`
and predictions `[0,3,9]` have score
`(log(2)^2)/3`, approximately `0.16015100463940046`.
A single target of 0 predicted as 1 has score `log(2)^2`.

## Contract

- Equal-length sequences of **1 to 10,000** finite real observations.
- Each target and prediction lies in **[0,1,000,000]**.
  Negative, missing, nonfinite and excessive values are invalid.
- Returns a finite, nonnegative scalar double; result is an
  unweighted arithmetic mean (not its square root).
- C accepts double arrays and a caller-owned output, which
  remains unchanged on failure and must not overlap inputs.
  Rust accepts slices and returns `Result<f64, &str>`.
  Python accepts lists/tuples but not bools or nonnumeric entries.
  R accepts plain numeric vectors, not matrices or complex values.
- No implicit clamping, sample weights, masks, or batching.

## Complexity

**O(n)** time and **O(1)** working memory for C and Rust;
Python allocates **O(n)** validated inputs, while R uses
**O(n)** temporaries for vectorized log transforms.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [mean_squared_log_error_c](mean_squared_log_error_c/) |
| Python | [mean_squared_log_error_python](mean_squared_log_error_python/) |
| Rust | [mean_squared_log_error_rust](mean_squared_log_error_rust/) |
| R | [mean_squared_log_error_r](mean_squared_log_error_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests check identical data, nontrivial pairs, zero targets,
bounded extreme values, invalid lengths, negative inputs,
nonnumeric and nonfinite entries.

## Limitations

MSLE cannot evaluate negative values and it does not replace
metrics in the original target scale. It can penalize
underpredictions asymmetrically on the original scale.
This educational implementation has no weights or gradient API.

## References

- Scikit-learn developers. (n.d.). *Metrics and scoring:
  Mean squared logarithmic error*. Scikit-learn.
  https://scikit-learn.org/stable/modules/model_evaluation.html
