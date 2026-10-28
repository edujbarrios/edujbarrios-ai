# Spearman Rank Correlation

## Why it belongs here

Spearman's rank correlation measures the strength and direction of a
*monotonic* association between two features. In contrast to Pearson
correlation on raw values, it first transforms each input into ranks,
making it useful for ordinal features and nonlinearly scaled data
(Spearman, 1904).

## Definition

For each observation, assign a **midrank**: if `L` entries are
strictly smaller and `E` entries are equal, its rank is
`L + (E + 1)/2`. Equal values thus receive the average rank they
would occupy in the sorted sequence. Compute the Pearson coefficient
of the two rank vectors:

```text
mean_rank = (n + 1) / 2
rho = sum((rx[i]-mean_rank)*(ry[i]-mean_rank)) /
      sqrt(sum((rx[i]-mean_rank)^2) * sum((ry[i]-mean_rank)^2))
```

Midranks are essential: the shortcut formula `1-6*sum(d^2)/(n*(n^2-1))`
is not generally valid when ties occur.

## Contract

- Two sequences of **equal length 2–128**, with finite real values in
  `[-1,000,000, 1,000,000]`. C and Rust accept double arrays;
  Python accepts lists or tuples, excluding booleans and nonnumeric
  objects; R accepts numeric vectors, excluding matrices.
- Exact equal values share a midrank. NaNs and infinities are rejected;
  missing observations are **not** silently dropped.
- A constant feature yields zero rank variance, so correlation is
  undefined and rejected. Other valid inputs produce a value in
  `[-1,1]`, clamped only for floating-point rounding.
- C returns a success flag and writes to a caller-owned output only
  on success (the output must not overlap either input).
  Python and R raise an error; Rust returns `Result<f64, &str>`.

Examples:

```text
x=[1,2,3], y=[4,5,6]          =>  1
x=[1,2,3], y=[6,5,4]          => -1
x=[1,1,2,3], y=[1,2,2,3]      =>  5/6
```

## Complexity

The reference deliberately computes midranks using pairwise comparisons:
**O(n²)** time and **O(n)** auxiliary memory. It avoids sorting and
external dependencies for educational clarity; it is not intended
for large datasets.

## Implementations and tests

| Language | Directory |
| --- | --- |
| C | [spearman_rank_correlation_c](spearman_rank_correlation_c/) |
| Python | [spearman_rank_correlation_python](spearman_rank_correlation_python/) |
| Rust | [spearman_rank_correlation_rust](spearman_rank_correlation_rust/) |
| R | [spearman_rank_correlation_r](spearman_rank_correlation_r/) |

Run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include positive, negative and tied ranks; a nonmonotonic
example; constant and singleton inputs; length mismatches; invalid
types and nonfinite/out-of-range entries.

## Limitations

Spearman measures monotonic association, not causation. Midranks treat
exact equality as ties, which may be sensitive to rounding of input
measurements. The implementation is unweighted, does not perform
hypothesis testing, and uses a quadratic teaching algorithm.

## References

- Spearman, C. (1904). The proof and measurement of association
  between two things. *The American Journal of Psychology, 15*(1),
  72–101. https://doi.org/10.2307/1412159
