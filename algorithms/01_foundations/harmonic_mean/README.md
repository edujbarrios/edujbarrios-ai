# Harmonic Mean (Scale-Stable)

## Foundations context

The harmonic mean is useful for averaging rates and ratios with a shared numerator, as well as balancing precision and recall in information retrieval. It gives more influence to small positive values than the arithmetic or geometric means (Hardy et al., 1952).

## Mathematical definition

For `n >= 1` finite nonnegative real observations:

```text
H(x) = n / sum(1/x[i])                  when all x[i] > 0
H(x) = 0                                 when any x[i] = 0
```

Direct reciprocal summation overflows for tiny positive values. Instead, let `a = min(x)` for strictly positive inputs, and compute the mathematically equivalent, scale-stable form:

```text
H(x) = a / ((sum(a/x[i])) / n)
```

Each ratio `a/x[i]` is between zero and one, so the sum does not overflow for the supported length. The minimum observation contributes exactly one to the sum. All input values are validated **before** applying the zero rule.

## Contract

- Input: a one-dimensional sequence of **1..512** finite **nonnegative** real observations. Zero values are permitted and make the mean zero.
- Reject negatives, NaN/infinity, lengths outside 1..512 and nonnumeric types. Python rejects booleans, complex values and malformed sequences; R rejects logical, complex, nonflat and missing values.
- Output is a single finite nonnegative floating-point value. Python returns `float`, Rust `Result<f64,&str>`, R a numeric scalar. Input is not modified.
- C takes `const double *values`, `size_t n`, `double *mean_out` and returns `bool`. Invalid input or a nonfinite result leaves `mean_out` unchanged.
- Finite floating-point rounding and underflow are possible; implementations never form the potentially overflowing `1/x` reciprocals.

## Complexity

**O(n)** time and **O(1)** auxiliary memory; the implementation scans for the minimum, then sums scaled ratios.

## Implementations

| Language | Folder |
| --- | --- |
| C | [harmonic_mean_c](harmonic_mean_c/) |
| Python | [harmonic_mean_python](harmonic_mean_python/) |
| Rust | [harmonic_mean_rust](harmonic_mean_rust/) |
| R | [harmonic_mean_r](harmonic_mean_r/) |

## Examples and tests

```text
harmonic_mean([2, 6])          = 3
harmonic_mean([1, 2, 4])       = 12/7
harmonic_mean([0, 2, 6])       = 0
harmonic_mean([1e-300,1e300]) ≈ 2e-300
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover zeros, singleton inputs, highly imbalanced magnitudes, maximum-size inputs and validation failures.

## Limitations

This is an unweighted harmonic mean, not a geometric mean or an `F_beta` score. It assumes rates are meaningfully comparable and nonnegative. Floating-point precision limits apply to extreme input ratios.

## References

Hardy, G. H., Littlewood, J. E., & Pólya, G. (1952). *Inequalities* (2nd ed.). Cambridge University Press.
