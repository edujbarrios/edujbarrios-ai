# Binary Cohen’s Kappa Agreement

## Why it belongs in AI

**Cohen's kappa (κ)** measures how consistently two raters label
the same examples, correcting their observed agreement for the
agreement expected by chance from their respective class frequencies.
It is used for human annotation quality, classifier–annotator
comparisons and dataset quality checks (Cohen, 1960).

Unlike accuracy, it accounts for chance agreement due to **class
imbalance**. It is not identical to Matthews correlation coefficient,
F1, or an inter-annotator measure designed for more than two raters.

## Formula

For n aligned **binary** labels `a[i],b[i]∈{0,1}`, let:

```text
A_0 = number of zeros in a; A_1 = number of ones in a
B_0 = number of zeros in b; B_1 = number of ones in b

p_o = number of exact matches / n
p_e = (A_0/n)*(B_0/n) + (A_1/n)*(B_1/n)

kappa = (p_o - p_e) / (1 - p_e)
```

A κ of **1** indicates perfect agreement when the denominator is
nonzero. Values near zero indicate chance-level agreement under
the empirical-marginal model, and negative values indicate less
than chance agreement.

Example:

```text
first  = [0,0,1,1]
second = [0,0,0,1]
p_o = 3/4 = 0.75
p_e = 0.5
kappa = 0.5
```

An all-zero first vector and all-zero second vector gives
`p_o=p_e=1`, so the formula becomes **0/0**. This reference
**rejects such degenerate input** instead of inventing κ=1.

## Shared contract

- Nonempty aligned label arrays of equal length, each item
  an exact integer **0 or 1**. Both raters have equal influence.
- Return a finite κ in `[-1,1]` where the empirical expected
  disagreement `1-p_e>0`.
- Reject malformed types, mismatched or empty vectors, nonbinary
  labels and undefined `p_e=1` cases. An all-zero first rater
  and all-one second rater has `p_e=0`, so is defined (κ=0).
- C takes caller-owned `uint8_t first[n], second[n]` arrays and
  a disjoint output `double* result`. On any failure the output
  is **unchanged**. Rust returns `Result<f64,...>`.
  Python/R raise errors and do not modify input.
- Python rejects bools as labels; R requires finite integer-valued
  numeric labels exactly 0/1. The result is a scalar, not a
  confusion matrix or uncertainty estimate.

## Numerical note

The calculation uses proportions before multiplication to avoid
overflow in integer count products. All floating intermediates
are checked for finiteness. The denominator check distinguishes
an **undefined perfect-class degeneracy** from an ordinary
highly imbalanced but valid case.

## Complexity

**O(n)** time, **O(1)** auxiliary space in all four languages.

## Implementations

| Language | Directory |
| --- | --- |
| C | [cohens_kappa_c](cohens_kappa_c/) |
| Python | [cohens_kappa_python](cohens_kappa_python/) |
| Rust | [cohens_kappa_rust](cohens_kappa_rust/) |
| R | [cohens_kappa_r](cohens_kappa_r/) |

Run from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover κ=0.5, κ=-1, perfect mixed-class agreement,
chance-level agreement, undefined all-one/all-zero agreement,
different constant-class raters and invalid data.

## Limitations

Binary and unweighted only. No weighted kappa, multiclass
extension, Fleiss' kappa, Krippendorff's alpha, confidence
interval, significance test or handling of missing labels.
Kappa is influenced by label prevalence; low kappa does not
necessarily imply low raw agreement on imbalanced datasets.
No CUDA or GPU performance claims.

## Reference

Cohen, J. (1960). A coefficient of agreement for nominal scales.
*Educational and Psychological Measurement, 20*(1), 37–46.
https://doi.org/10.1177/001316446002000104
