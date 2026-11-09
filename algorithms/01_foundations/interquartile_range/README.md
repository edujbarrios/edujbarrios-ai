# Interquartile Range (Type 7 Quantiles)

## Foundations context

The **interquartile range (IQR)** is a robust measure of spread, defined as the difference between the upper and lower quartiles. Unlike standard deviation, it is much less sensitive to a small number of outliers (Hyndman & Fan, 1996).

## Definition

Sort finite observations `x[0] <= ... <= x[n-1]` and use the **type 7 quantile** rule (the default quantile convention in R). For `p` in `{0.25, 0.75}`, define:

```text
h = (n - 1) * p
i = floor(h)
a = h - i
Q(p) = x[i]                              if a = 0
Q(p) = (1 - a) * x[i] + a * x[i+1]        otherwise
IQR = Q(0.75) - Q(0.25)
```

Weighted interpolation avoids calculating `x[i+1]-x[i]` first, which could overflow on extreme opposite-sign observations. A mathematically representable IQR may still be subject to ordinary rounding. A nonfinite computed IQR is rejected.

## Contract

- Input: a flat sequence of **1..512 finite real observations**, including negative values, duplicates, and zeros.
- Return a **finite nonnegative scalar** IQR; a single observation or constant-valued sequence returns zero.
- C accepts `const double *values`, `size_t n` and `double *iqr_out`; on any invalid input or nonfinite result it returns `false` and leaves the caller's output untouched.
- Python accepts a `list` or `tuple` of real numeric values and returns `float`. Rust returns `Result<f64,&str>`. R accepts a numeric vector and returns one numeric scalar. Python rejects booleans, wrong types and nonfinite values; R rejects logical, complex, matrix/array, missing and nonfinite values. Inputs are not mutated.
- The precise quartile convention is important: other quantile definitions (such as Tukey hinges) can give different IQRs for small samples.

## Complexity

Sorting dominates: **O(n log n)** time and **O(n)** workspace for a copy of the input.

## Examples and tests

```text
IQR([1,2,3,4])           = 1.5     # Q1=1.75, Q3=3.25
IQR([1,2,3,4,100])       = 2.0     # Q1=2, Q3=4
IQR([1,5])               = 2.0     # Q1=2, Q3=4
IQR([42])                = 0.0
IQR([-1e308,1e308])     ≈ 1e308  # finite
```

Implementations are in [C](interquartile_range_c/), [Python](interquartile_range_python/), [Rust](interquartile_range_rust/), and [R](interquartile_range_r/). From the repository root, run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`.

Tests cover quartile interpolation, tie-heavy and unsorted data, singleton, 512-element boundary, extreme finite observations, nonfinite-result rejection and invalid inputs.

## Limitations

IQR alone does not characterize skew or multimodality. Its value depends on the chosen sample-quantile interpolation convention. Floating-point calculations have finite precision, and some finite samples have an unrepresentable IQR exceeding the floating-point maximum.

## References

Hyndman, R. J., & Fan, Y. (1996). Sample quantiles in statistical packages. *The American Statistician, 50*(4), 361–365. https://doi.org/10.1080/00031305.1996.10473566
