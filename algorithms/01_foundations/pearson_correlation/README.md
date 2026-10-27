# Pearson Correlation Coefficient

## Why it belongs here

Pearson correlation quantifies the strength and direction of a *linear*
association between paired numeric features. It is useful for exploratory
feature analysis and for understanding covariance-based statistics.

## Definition

For two aligned sequences of length `n`, calculate the means `mx`, `my`
and centered sums `Sxx`, `Syy`, and `Sxy`:

```text
Sxx = sum((x[i] - mx)^2)
Syy = sum((y[i] - my)^2)
Sxy = sum((x[i] - mx) * (y[i] - my))
r = Sxy / sqrt(Sxx * Syy)
```

The implementation evaluates `Sxy / sqrt(Sxx) / sqrt(Syy)` to avoid
multiplying the two sums. Values within rounding error of the range
are clamped to [-1, 1]. This is the standard product-moment coefficient.

## Contract

- Both input sequences must have the **same length, 2 to 10,000**.
- Every entry must be a finite real number in **[-1,000,000, 1,000,000]**;
  booleans and nonnumeric objects are rejected by Python.
- Both sequences must have nonzero centered sum of squares. Constant
  inputs are undefined and rejected, not assigned a correlation of zero.
- Returns one finite value in [-1, 1]. A positive value indicates a
  positive linear relation; a negative value indicates an inverse one.
- C returns a success flag with a caller-owned output left unchanged
  on failure; its input arrays must not overlap the output. Rust returns
  `Result<f64, &str>`. Python and R raise errors for invalid input.
- Equal-valued pairs and the first/last entries count independently;
  there is no missing-value imputation.

For example, `x=[1,2,3]` and `y=[2,4,6]` give `r=1`; reversing
`y` gives `r=-1`. The vectors `[1,2,3]` and `[1,0,1]` give `r=0`.

## Complexity

Two passes over the data: **O(n)** time and **O(1)** auxiliary
working memory for C, Rust and R (Python stores checked floats, O(n)).

## Implementations

| Language | Folder |
| --- | --- |
| C | [pearson_correlation_c](pearson_correlation_c/) |
| Python | [pearson_correlation_python](pearson_correlation_python/) |
| Rust | [pearson_correlation_rust](pearson_correlation_rust/) |
| R | [pearson_correlation_r](pearson_correlation_r/) |

Run repository checks with:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The reference suites cover positive, negative and zero correlation;
two-point inputs, unequal lengths, constant features, invalid types,
nonfinite values and values outside the declared range.

## Limitations

Only *linear* association is measured; correlation does not imply
causation. This two-pass floating-point implementation can lose
precision on nearly constant data, and it deliberately rejects very
large magnitudes. No confidence intervals, missing values, ranks,
weights or SIMD/GPU optimizations are provided.

## References

- NIST/SEMATECH. (2013). *e-Handbook of statistical methods*.
  National Institute of Standards and Technology.
  https://www.itl.nist.gov/div898/handbook/
