# Median Absolute Deviation (MAD)

## Why it belongs in this repository

Median absolute deviation is a robust measure of numerical dispersion useful in outlier detection, resilient feature scaling, and data preprocessing. It is less sensitive to extreme observations than the standard deviation (Rousseeuw & Croux, 1993).

## Mathematical definition

For nonempty finite values `x[0],...,x[n-1]`, define

```text
center = median(x)
MAD    = median(|x[i] - center| for i in 0..n-1)
```

For even `n`, the median is the arithmetic mean of the two middle sorted values. This implementation returns the **raw (unscaled)** MAD, not the normally consistent estimate `1.4826 × MAD`.

## Contract

- Input: a nonempty sequence of **1–512 finite real values**; Python accepts a list/tuple of real `int` or `float`, but rejects booleans, complex values and nonfinite numbers. R accepts a flat numeric vector, excluding logical, complex and nonfinite values.
- Output: one nonnegative finite double/float. Identical values, including a singleton, return 0.
- Reject missing data, invalid input types, an empty sequence, lengths above 512 and any nonfinite intermediate difference or output. No imputation or clipping.
- C accepts `const double *values`, `size_t n`, `double *mad_out`; it leaves the output unchanged if validation fails. Rust returns `Result<f64, &str>`; Python returns `float`; R returns one numeric scalar.
- No mutation of the input. Sorting uses copies. The even-length median uses guarded arithmetic to limit overflow for extreme finite inputs.

## Complexity

Sorting the observations and deviations takes **O(n log n)** time, using **O(n)** auxiliary memory; the C implementation uses two fixed 512-element buffers on the stack.

## Implementations

| Language | Folder |
| --- | --- |
| C | [median_absolute_deviation_c](median_absolute_deviation_c/) |
| Python | [median_absolute_deviation_python](median_absolute_deviation_python/) |
| Rust | [median_absolute_deviation_rust](median_absolute_deviation_rust/) |
| R | [median_absolute_deviation_r](median_absolute_deviation_r/) |

## Examples and tests

```text
MAD([1,1,2,2,4,6,9]) = 1
MAD([1,2,3,10])       = 1
MAD([5,5,5])          = 0
MAD([42])             = 0
```

From the root, run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`. Tests cover even/odd counts, reordered observations, outliers, sign changes, boundaries, large finite values, invalid numeric inputs, and overflow.

## Limitations

Raw MAD is not the population standard deviation, and it is not directly comparable to an unscaled standard deviation. For Gaussian scale estimation, a separate consistency factor is required. MAD may be zero for highly repeated observations and does not measure the direction of deviation.

## References

Rousseeuw, P. J., & Croux, C. (1993). Alternatives to the median absolute deviation. *Journal of the American Statistical Association, 88*(424), 1273–1283. https://doi.org/10.1080/01621459.1993.10476408
