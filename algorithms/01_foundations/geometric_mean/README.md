# Geometric Mean (Log Domain)

## AI and statistics context

The geometric mean summarizes multiplicative growth, positive ratios and proportional factors. Unlike the arithmetic mean, it treats a product of factors symmetrically and is useful in numerical preprocessing and evaluation when measurements are multiplicative (Aitchison, 1986).

## Mathematical definition

For `n >= 1` finite nonnegative observations `x[i]`:

```text
GM(x) = (product(x[i]))^(1/n)
      = exp(sum(log(x[i])) / n)   if all x[i] > 0
      = 0                          if any x[i] = 0
```

The implementations use **log-domain** accumulation for strictly positive values to avoid overflow or underflow from directly multiplying all observations. A zero factor produces exactly 0 after validating every input.

## Contract

- Input: **1..512** finite **nonnegative** real numbers; includes zero and subnormal positive values. Reject negative numbers, NaN, infinity, invalid types, booleans (Python), logical/complex/matrix inputs (R), and lengths outside the range.
- Output: one finite, nonnegative floating-point number. It is allowed to have ordinary floating-point rounding error, and tests compare approximate results with tolerances.
- C accepts `const double *values`, `size_t n`, `double *mean_out`. On invalid inputs or nonfinite intermediate results it returns `false` and leaves `mean_out` unchanged. Rust returns `Result<f64,&str>`; Python returns `float`; R returns one numeric scalar.
- The input is not mutated. A singleton returns its numeric value up to floating-point rounding.

## Complexity

**O(n)** time and **O(1)** additional memory; the algorithm does not store or sort the input.

## Implementations

| Language | Folder |
| --- | --- |
| C | [geometric_mean_c](geometric_mean_c/) |
| Python | [geometric_mean_python](geometric_mean_python/) |
| Rust | [geometric_mean_rust](geometric_mean_rust/) |
| R | [geometric_mean_r](geometric_mean_r/) |

## Examples and tests

```text
GM([2, 8])           = 4
GM([1, 1, 1])        = 1
GM([0, 2, 8])        = 0
GM([1e-300, 1e300])  ≈ 1
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover zeros, singletons, noninteger factors, very large/small finite values, invalid inputs and the 512-item boundary.

## Limitations

The geometric mean requires nonnegative data and is not a substitute for an arithmetic mean. It loses information about the ordering of factors, and its logarithmic implementation incurs rounding error. It does not support negative factors or complex-valued roots.

## References

Aitchison, J. (1986). *The statistical analysis of compositional data*. Chapman & Hall.
