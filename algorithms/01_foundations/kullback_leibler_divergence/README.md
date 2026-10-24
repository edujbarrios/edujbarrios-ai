# Discrete Kullback–Leibler Divergence (Base 2)

## Motivation

**Kullback–Leibler (KL) divergence**, also called *relative entropy*,
measures how differently two probability distributions assign mass
to the same outcomes. It underlies information theory,
probabilistic model fitting, Bayesian methods and variational
inference (Kullback & Leibler, 1951).

This reference uses **base-2 logarithms** (bits). KL is **directional**:
`D_KL(P||Q)` generally differs from `D_KL(Q||P)`.
It is not a metric and need not be bounded.

## Mathematical definition

For two aligned categorical distributions `P,Q`:

```text
D_KL,2(P || Q) = sum_i P_i * log2(P_i / Q_i)
```

Special cases are essential:

- When `P_i=0`, the contribution is defined as **zero**,
  including `P_i=Q_i=0`.
- When `P_i>0` and `Q_i=0`, the divergence is **positive
  infinity**, not a malformed input. All input values must still
  be validated before returning that mathematical result.

Examples:

```text
P=[.5,.5], Q=[.5,.5] -> 0 bits
P=[1,0], Q=[.5,.5]   -> 1 bit
P=[.75,.25], Q=[.5,.5] -> 0.1887218755408672 bits
P=[1,0], Q=[0,1]     -> +infinity
P=[0,1], Q=[.5,.5]   -> 1 bit
```

Due to the directionality, swapping the two distributions may
change the result. For a symmetric finite alternative, see this
repository's Jensen–Shannon divergence.

## Shared contract

- Both arrays have equal, **nonempty** length `1..4096`.
  Each entry is a finite real in `[0,1]`.
- Both distributions must sum to 1 within **absolute tolerance
  1e-9**. There is no silent normalization or smoothing.
- Return a nonnegative KL divergence measured in **bits**.
  Positive infinity is a **valid result** when P places positive
  mass outside Q's support. Tiny negative values from floating
  rounding are clamped to zero.
- Reject NaN/Inf **inputs**, negative values, entries above one,
  invalid types, shape mismatches, nonnormalized vectors, or
  arithmetic overflow not explained by support mismatch.
- C accepts `double p[n],q[n]` and caller-owned `double* result`,
  with properly sized nonoverlapping buffers. On invalid input or
  non-support-related arithmetic failure the output stays unchanged.
  On missing support, C returns `true` and stores `INFINITY`.
  Rust returns `Ok(f64::INFINITY)` for missing support and
  `Err` for invalid data. Python/R return positive infinity for
  missing support and raise errors for invalid inputs.
- Python rejects boolean numbers; R requires base numeric arrays.
  The probability coordinates are paired by **category order**,
  not sorted by their numeric values.

## Complexity

**O(n)** time, **O(1)** auxiliary memory across languages.

## Implementations

| Language | Directory |
| --- | --- |
| C | [kullback_leibler_divergence_c](kullback_leibler_divergence_c/) |
| Python | [kullback_leibler_divergence_python](kullback_leibler_divergence_python/) |
| Rust | [kullback_leibler_divergence_rust](kullback_leibler_divergence_rust/) |
| R | [kullback_leibler_divergence_r](kullback_leibler_divergence_r/) |

Run tests from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover identity, directional nonuniform distributions,
zero-valued entries, support mismatch, malformed distributions,
nonfinite input values and shape differences.

## Limitations

Discrete finite, aligned outcome categories only. The algorithm
does not infer categories from samples or estimate density models,
and no epsilon smoothing is applied. Near-zero Q entries can
produce extremely large divergences. The tolerance for probability
normalization does not guarantee that a near-normalized input is
an exact probability distribution. No CUDA implementation is claimed.

## References

Kullback, S., & Leibler, R. A. (1951). On information and sufficiency.
*The Annals of Mathematical Statistics, 22*(1), 79–86.
https://doi.org/10.1214/aoms/1177729694
