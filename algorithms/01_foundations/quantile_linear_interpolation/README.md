# Type-7 Linear Quantile Interpolation

## Foundations context

Quantiles summarize distributions at configurable probability levels, from the minimum and maximum to medians and percentiles. This implementation follows **type 7**, the default R sample quantile definition and the convention used by the catalogue's `interquartile_range` algorithm (Hyndman & Fan, 1996).

## Mathematical definition

Given `n` finite observations sorted in ascending order `x[0] <= … <= x[n-1]` and `p` in `[0,1]`, compute:

```text
h = (n-1) p
i = floor(h)
f = h-i
Q(p) = x[i]                                     if f = 0
Q(p) = (1-f)*x[i] + f*x[i+1]                    otherwise
```

Weighted interpolation avoids computing a possibly overflowing `x[i+1] - x[i]`. The result is required to be finite. The endpoints are exactly the minimum and maximum; the median is `p=0.5`. For `n=1`, every valid probability returns the sole sample.

## Contract

- Input: **1–512 finite real observations** and a **finite real probability between 0 and 1 inclusive**.
- Output: a finite floating-point scalar. The input is not mutated. Floating-point results are compared with tolerances, not exact equality.
- C accepts `const double *values`, `size_t n`, `double probability`, and `double *quantile_out`, returning `bool`. Invalid arguments or a nonfinite result leave the output untouched.
- Python takes a list or tuple of real numbers and a real probability, rejects booleans, malformed types and infinities/NaNs, and returns `float`. Rust returns `Result<f64, &str>`; R returns one numeric scalar and rejects logical, complex, matrix and missing values.
- This algorithm interpolates **sample quantiles**. It is not a weighted quantile or a model-calibrated predictive interval.

## Complexity

Sorting a copy takes **O(n log n)** time and **O(n)** extra space; the quantile lookup is O(1).

## Examples

```text
Q([1,2,3,4], 0.25) = 1.75
Q([1,2,3,4], 0.50) = 2.50
Q([1,2,3,4], 0.75) = 3.25
Q([5,10], 0.20)    = 6
Q([42], 0.90)      = 42
```

## Implementations and testing

| Language | Folder |
| --- | --- |
| C | [quantile_linear_interpolation_c](quantile_linear_interpolation_c/) |
| Python | [quantile_linear_interpolation_python](quantile_linear_interpolation_python/) |
| Rust | [quantile_linear_interpolation_rust](quantile_linear_interpolation_rust/) |
| R | [quantile_linear_interpolation_r](quantile_linear_interpolation_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover quartiles, the median, endpoint quantiles, singleton, maximum length, out-of-range probabilities, invalid observations, and large opposite-sign values.

## Limitations

Different quantile conventions yield different sample estimates. Extreme finite values may lose low-order precision in interpolation, even though the type-7 result must remain within the data range mathematically.

## References

Hyndman, R. J., & Fan, Y. (1996). Sample quantiles in statistical packages. *The American Statistician, 50*(4), 361–365. https://doi.org/10.1080/00031305.1996.10473566
