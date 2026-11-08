# Symmetric Trimmed Mean

## Foundations context

A trimmed mean reduces sensitivity to extreme observations by removing a configurable fraction of the smallest and largest values before averaging. It is a robust location estimator often used alongside medians and winsorized estimators (Wilcox, 2012).

## Mathematical definition

For `n` finite real observations and a trim fraction `f` in **[0,0.5)**, sort the input as `x[0] <= ... <= x[n-1]`. Let `k = floor(n × f)`. Drop exactly `k` observations from **each end**, leaving `n-2k` values. Return:

```text
trimmed_mean(x, f) = (x[k] + ... + x[n-k-1]) / (n - 2k)
```

The implementation sums scaled contributions `x[i]/(n-2k)` rather than forming a potentially overflowing unscaled sum. It validates that all computed intermediate results remain finite.

## Contract

- `values` is a flat sequence of **1..512** finite real observations. Python rejects bools, complex values, strings, non-sequences and nonfinite numbers. R rejects logical, complex, matrix, missing or nonfinite input.
- `trim_fraction` is a **single finite real number in [0,0.5)** (not boolean). Invalid or missing fractions are rejected.
- The output is a finite real **population sample location**, not a variance. With `f=0`, it is the arithmetic mean; with large `f`, the remaining observations approach the median. The input order and data are not modified.
- C takes `const double *values`, `size_t n`, `double trim_fraction` and `double *mean_out`, and returns `bool`. It leaves `mean_out` unchanged if input validation or a finite-intermediate check fails. Rust returns `Result<f64,&str>`, Python returns `float`, R one numeric scalar.
- For `n=1`, any valid fraction returns the only value. For even `n`, `f<0.5` leaves at least two middle values.

## Complexity

Sorting takes **O(n log n)** time and averaging takes **O(n)** time; extra storage is **O(n)** for a sorted copy, bounded at 512 elements.

## Implementations

| Language | Folder |
| --- | --- |
| C | [trimmed_mean_c](trimmed_mean_c/) |
| Python | [trimmed_mean_python](trimmed_mean_python/) |
| Rust | [trimmed_mean_rust](trimmed_mean_rust/) |
| R | [trimmed_mean_r](trimmed_mean_r/) |

## Examples and tests

```text
trimmed_mean([1,2,3,4,100], 0.2) = 3
trimmed_mean([1,2,3,100],   0.25) = 2.5
trimmed_mean([1,2,3,100],   0) = 26.5
trimmed_mean([42],          0.49) = 42
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`. Tests cover boundary fractions, unsorted inputs, duplicate/extreme values, lengths 1 and 512, invalid types, overflow and fraction values at/outside 0.5.

## Limitations

The number trimmed is `floor(nf)`, which can be zero for small samples. This is an **unweighted**, symmetric trimmed mean; no winsorization, missing-value imputation, asymmetrical tail adjustment or standard error is provided. Floating-point rounding still applies.

## References

Wilcox, R. R. (2012). *Introduction to robust estimation and hypothesis testing* (3rd ed.). Academic Press.
