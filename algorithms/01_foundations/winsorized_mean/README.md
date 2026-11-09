# Symmetric Winsorized Mean

## Foundations context

A winsorized mean reduces the influence of extreme numerical observations without deleting samples. Unlike a trimmed mean, it **replaces** tail observations with the corresponding retained boundary values, then averages all observations (Wilcox, 2012).

## Mathematical definition

For sorted finite observations `x[0] <= ... <= x[n-1]`, and a trim fraction `f` in `[0,0.5)`, set `k = floor(n*f)`. Create a winsorized sequence `w`:

```text
w[i] = x[k]              if i < k
w[i] = x[i]              if k <= i < n-k
w[i] = x[n-k-1]          if i >= n-k
winsorized_mean(x,f) = sum(w) / n
```

To avoid overflow of the unscaled sum, implementations accumulate scaled terms `w[i]/n`. Each intermediate result must remain finite.

## Contract

- Input `values`: a one-dimensional sequence of **1..512 finite real** numbers; `trim_fraction`: a finite real scalar in **[0,0.5)**.
- Python rejects boolean, complex, nonnumeric, nonfinite or malformed observations/fractions. R rejects logical/complex/matrix/missing values and invalid fractions.
- Returns a **finite real scalar**; the original observations remain unchanged. When `f=0`, the result is the ordinary arithmetic mean. For `n=1`, any valid `f` returns the single value.
- C accepts `const double *values`, `size_t n`, `double trim_fraction`, `double *mean_out`. It returns `false` and leaves the output **unchanged** on invalid input or nonfinite intermediates. Rust returns `Result<f64,&str>`; Python returns a `float`; R a numeric scalar.
- Floating-point rounding is expected. Tests use tolerances for nonexact decimal means.

## Complexity

Sort a copy in **O(n log n)** time, then accumulate **O(n)** contributions. Extra storage **O(n)**, bounded by 512 elements.

## Implementations

| Language | Folder |
| --- | --- |
| C | [winsorized_mean_c](winsorized_mean_c/) |
| Python | [winsorized_mean_python](winsorized_mean_python/) |
| Rust | [winsorized_mean_rust](winsorized_mean_rust/) |
| R | [winsorized_mean_r](winsorized_mean_r/) |

## Examples and tests

```text
winsorized_mean([1,2,3,4,100],0.2) = 3.0   # [2,2,3,4,4]
winsorized_mean([1,2,3,100],0.25)  = 2.5   # [2,2,3,3]
winsorized_mean([1,2,3,100],0)     = 26.5
winsorized_mean([1,5,100],0.49)    = 5.0
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the root. Tests cover odd/even lengths, unsorted input, exact tail replacement, singleton, maximum length, overflow-safe extreme magnitudes and invalid/nonfinite input.

## Limitations

The number of observations replaced per tail is `floor(nf)`, so small sample sizes may result in no replacement. This implementation is symmetric, unweighted and not a robust standard-error estimator. Binary floating-point rounding can affect the final low-order bits.

## References

Wilcox, R. R. (2012). *Introduction to robust estimation and hypothesis testing* (3rd ed.). Academic Press.
