# Online Mean and Population Variance

**Stage:** 01 — Foundations · **Languages:** C, Python, Rust, R.

## Why it belongs in this repository

Feature standardization, anomaly detection, streaming data summaries, and
distribution monitoring frequently need a mean and variance. Computing
`sum(x²) / n - mean²` can suffer catastrophic cancellation when the
observations cluster around a large common value. This implementation
uses Welford's online update to maintain a centered sum of squares (Welford,
1962), connecting numerical statistics to later classical ML algorithms.

## What it does

For `n` finite real observations:

```text
population_mean = (sum x_i) / n
population_variance = (sum (x_i - population_mean)²) / n
```

The update for each new observation `x` is:

```text
count += 1
delta = x - mean
mean += delta / count
M2 += delta * (x - mean)
variance = M2 / count
```

For `[2, 4, 4, 4, 5, 5, 7, 9]`, the mean is `5` and **population**
variance is `4`. The divisor is `n`, **not** `n - 1` (sample variance).

## Shared contract

- Input: a **nonempty** sequence of finite real observations.
- Output: a finite pair `(mean, population_variance)`; variance >= 0.
- Empty input, NaN or infinity: explicit error.
- A single observation has zero variance; constant series have zero variance.
- **Finite inputs alone do not guarantee representable intermediates.**
  If subtraction, squared updates, or the reported mean/variance become
  non-finite, return an error rather than silently reporting a result.
- C receives an array pointer, count, and two **distinct** output pointers.
  Caller guarantees an allocation of at least `n` elements. On failure
  neither output is changed. Python raises ValueError; Rust returns Result;
  R raises an error and returns a named list on success.
- Floating-point rounding is unavoidable. These tests use a tolerance for
  non-exact results.

## Complexity

One pass: **O(n)** time, **O(1)** auxiliary storage in all four languages,
excluding the caller-provided input and return values.

## Implementations

| Language | Location |
| --- | --- |
| C | [mean_variance_c](mean_variance_c/) |
| Python | [mean_variance_python](mean_variance_python/) |
| Rust | [mean_variance_rust](mean_variance_rust/) |
| R | [mean_variance_r](mean_variance_r/) |

CUDA is omitted: this educational **online** primitive is best introduced
with serial updates. Parallel numerically stable aggregation would be a
different, explicitly benchmarked extension.

## Examples and tests

```text
mean_variance([2, 4, 4, 4, 5, 5, 7, 9]) -> (5, 4)
mean_variance([3])                      -> (3, 0)
mean_variance([])                       -> error
```

Run from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

This is not an arbitrary-precision statistics package. The online recurrence
can still overflow for extreme opposing observations (e.g., values near the
limits of 64-bit floats); the implementation then errors. It does not provide
sample variance, weighted moments, multivariate covariance or an API for
merging independent partitions.

## References

Welford, B. P. (1962). Note on a method for calculating corrected sums of
squares and products. *Technometrics, 4*(3), 419–420.
https://doi.org/10.1080/00401706.1962.10490022
