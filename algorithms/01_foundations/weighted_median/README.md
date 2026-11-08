# Weighted Median

## Why it belongs in this repository

Weighted medians summarize a distribution when observations have unequal reliability, importance or frequency. They also appear in robust estimation and weighted L1 optimization, where squared-error averages can be too sensitive to large deviations (Cormen et al., 2022).

## Mathematical definition

Sort the pairs `(value, weight)` in ascending order by value, keeping duplicates. With finite nonnegative weights `w[i]` and positive finite total `W = sum(w)`, return the **smallest observed value** for which:

```text
cumulative_weight(value) >= W/2
```

A weighted median is not necessarily unique; this fixed **lower weighted median** convention chooses the smallest threshold-crossing observation. When the cumulative weight is exactly half, the lower value is selected. Zero-weight observations may be supplied but never trigger a match before positive cumulative weight is reached.

## Contract

- Input: two equal-length one-dimensional sequences of **1..512** real numbers. All values and weights must be finite; weights must be **nonnegative**, and their sum must be finite and strictly positive.
- Output: a **single observed input value**, as a finite double/float. The input is not mutated. Ties in values do not change the result.
- Reject empty/mismatched lengths, invalid types, booleans (Python), missing/nonfinite numbers, negative weights, all-zero weights and an overflowed weight sum.
- For numerical safety, threshold crossing is checked with `cumulative >= total - cumulative` instead of forming `total/2`, avoiding underflow for very small positive weights.
- C takes `const double *values`, `const double *weights`, `size_t n`, and `double *median_out`; returns `bool` and leaves the output untouched on failure. Rust returns `Result<f64, &str>`; Python returns `float`; R returns one numeric scalar.

## Complexity

Sort pairs by value in **O(n log n)** time using **O(n)** additional storage; find the first threshold crossing in **O(n)** time. The C implementation uses a fixed 512-entry array.

## Implementations

| Language | Folder |
| --- | --- |
| C | [weighted_median_c](weighted_median_c/) |
| Python | [weighted_median_python](weighted_median_python/) |
| Rust | [weighted_median_rust](weighted_median_rust/) |
| R | [weighted_median_r](weighted_median_r/) |

## Examples and tests

```text
weighted_median([1,2,10], [1,1,8])  = 10
weighted_median([1,2,3,4], [1,1,1,1]) = 2   # lower median at 50%
weighted_median([10,1,3], [0,2,1])  = 1
weighted_median([-5,2], [1,1])      = -5
```

From the repository root, run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`. Tests cover unsorted pairs, repeated values, equal-weight boundaries, zero weights, tiny and huge finite values, maximum length, invalid data and overflow.

## Limitations

This implementation provides the **lower** weighted median only; it does not interpolate, compute a weighted mean or expose a quantile API. Floating-point summation may lose precision for extremely imbalanced weights. No online updates or GPU variant are included.

## References

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to algorithms* (4th ed.). MIT Press.
