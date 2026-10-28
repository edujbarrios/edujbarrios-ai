# Kendall Tau-b Rank Correlation

## Why it belongs here

Kendall's tau-b is a nonparametric measure of monotonic association
between paired ordinal or numerical features. Unlike a simple count
of rank agreement, tau-b adjusts its denominator for tied pairs
(Kendall, 1945). It complements the Pearson and Spearman correlations
already in the catalogue.

## Definition

For each unordered pair of observations `i < j`, compare
`x[i]` against `x[j]` and `y[i]` against `y[j]`:

- Concordant (`C`): both differences have the same nonzero sign.
- Discordant (`D`): their nonzero signs differ.
- Tied in x only (`Tx`): x values equal, y values differ.
- Tied in y only (`Ty`): y values equal, x values differ.
- Tied in both: excluded from both denominator factors.

Then

```text
tau_b = (C - D) / sqrt((C + D + Tx) * (C + D + Ty))
```

An all-tied feature makes the denominator zero, so tau-b is
undefined. The result is clamped to [-1, 1] only for rounding.

Examples: `[1,2,3]` against `[2,3,4]` gives 1;
against `[4,3,2]` gives -1. Comparing
`[1,1,2,3]` with `[1,2,2,3]` gives **0.8**.

## Contract

- Both sequences must have the same length `2..128`.
- Values must be finite real numbers in `[-1,000,000,1,000,000]`.
  Exact equality defines a tie; no missing-value imputation.
- C and Rust receive double arrays; Python accepts lists/tuples
  (rejecting booleans and nonnumerical values); R receives numeric
  vectors (not matrices).
- On invalid/undefined input, C returns false without changing
  the caller-owned output (which must not overlap inputs);
  Rust returns `Err`; Python and R raise an error.

## Complexity

**O(n²)** time to classify all pairs; **O(1)** extra working
memory in all four language implementations.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [kendall_tau_b_c](kendall_tau_b_c/) |
| Python | [kendall_tau_b_python](kendall_tau_b_python/) |
| Rust | [kendall_tau_b_rust](kendall_tau_b_rust/) |
| R | [kendall_tau_b_r](kendall_tau_b_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover concordance, discordance, mixed order, both types
of ties, all ties, invalid lengths, nonfinite and out-of-range values.

## Limitations

This reference uses the quadratic pair-count method, not
an `O(n log n)` inversion-count implementation. It provides no
p-values, confidence intervals, weights, or tolerance for
near-equal floating-point values. Correlation is not causation.

## References

- Kendall, M. G. (1945). The treatment of ties in ranking problems.
  *Biometrika, 33*(3), 239–251.
  https://doi.org/10.1093/biomet/33.3.239
