# Root Mean Square (Scaled)

## Foundations context

Root mean square (RMS), also called the quadratic mean, summarizes the magnitude of signed observations. RMS is used in signal processing, error metrics, and analyzing feature amplitudes (Strang, 2016).

## Mathematical definition

For `n` finite real values `x[i]`, `RMS(x) = sqrt(sum(x[i]^2) / n)`. Direct squaring can overflow even when the answer is representable. Instead, let `s = max(abs(x[i]))` and calculate:

```text
RMS(x) = 0                                  if s = 0
RMS(x) = s * sqrt(sum((x[i]/s)^2) / n)        otherwise
```

Each normalized magnitude is at most one; thus all intermediate squares and their sum are bounded for `n <= 512`. The output is nonnegative. No input is mutated.

## Contract

- Input is a **flat sequence of 1..512 finite real observations**, including negative and zero values.
- Python rejects booleans, nonnumeric values, nonfinite values, and invalid collections. R rejects logical, complex, matrix/array, missing, and nonfinite values.
- Returns a finite, nonnegative floating-point RMS. C takes `const double *values`, `size_t n`, and `double *rms_out`; returns `false` for invalid inputs and leaves `rms_out` unchanged on failure. Rust returns `Result<f64,&str>`, Python returns `float`, R one numeric scalar.
- Results are subject to normal IEEE-754 rounding. This is **not** the population standard deviation because values are not centered.

## Complexity

Time **O(n)** and extra working memory **O(1)**, aside from a temporary checked Python list.

## Implementations

| Language | Folder |
| --- | --- |
| C | [root_mean_square_c](root_mean_square_c/) |
| Python | [root_mean_square_python](root_mean_square_python/) |
| Rust | [root_mean_square_rust](root_mean_square_rust/) |
| R | [root_mean_square_r](root_mean_square_r/) |

## Examples and tests

```text
RMS([3, 4])              = sqrt(12.5)
RMS([-3, 4])             = sqrt(12.5)
RMS([0, 0, 0])           = 0
RMS([1e308, -1e308])    = 1e308  # no intermediate overflow
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` at the repository root. Tests include the single-value and 512-value bounds, finite extremes, sign invariance and input validation.

## Limitations

No weights, centering or missing-value imputation are supported. The normalized accumulation reduces but does not eliminate floating-point rounding, particularly for inputs with large differences in magnitude.

## References

Strang, G. (2016). *Introduction to linear algebra* (5th ed.). Wellesley-Cambridge Press.
