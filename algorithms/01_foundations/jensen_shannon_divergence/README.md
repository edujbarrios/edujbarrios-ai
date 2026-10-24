# Jensen–Shannon Divergence (Base 2)

## Why it belongs in AI

**Jensen–Shannon divergence (JSD)** measures the similarity of
two probability distributions. Unlike raw Kullback–Leibler divergence,
it is symmetric and always finite for finite discrete probability
vectors, including when one distribution assigns zero probability
to an outcome. It appears in machine learning, information theory,
model comparison and distribution-shift studies (Lin, 1991).

This reference computes **the divergence, not its square root**.
Its square root is often called the Jensen–Shannon distance.

## Mathematical definition

For discrete distributions `P,Q` sharing the same support,
define `M_i=(P_i+Q_i)/2`.

```text
JSD_2(P,Q) = 0.5 * KL_2(P || M) + 0.5 * KL_2(Q || M)

KL_2(A || B) = sum_i A_i * log2(A_i / B_i)
```

Interpret the term with `A_i=0` as **zero**, even if `B_i=0`.
For nonzero `A_i`, its mixture entry `M_i` is always positive.

Base-2 logarithms give `JSD_2 ∈ [0,1]` for two distributions.
Some sources use the natural logarithm and obtain a maximum of
`ln(2)`. That choice changes the numerical result.

Examples:

```text
P=[1,0], Q=[0,1]      -> JSD_2 = 1
P=[0.5,0.5], Q=same   -> JSD_2 = 0
P=[1,0], Q=[0.5,0.5] -> JSD_2 ≈ 0.311278124459
```

## Shared contract

- Input distributions are nonempty **same-length** arrays of
  probabilities with **1 <= n <= 4096** entries.
- Each entry must be a finite real in `[0,1]`, and each vector's
  total probability must be within **absolute tolerance 1e-9** of 1.
  The algorithm does **not** renormalize inputs.
- Return a finite scalar in `[0,1]`, permitting a tiny numerical
  tolerance in intermediate results and clipping only floating-point
  noise at the final boundaries.
- Reject negative or nonfinite probabilities, inconsistent lengths,
  unnormalized distributions, and invalid input types.
- C reads `double p[n], q[n]` and writes to a caller-owned
  `double* result` only after successful validation/computation.
  Rust returns `Result<f64,...>`; Python/R raise errors.
  C buffers must be sized correctly and not overlap.
- Zero-probability terms contribute exactly zero and never produce
  `log2(0)`. Python disallows bools; R accepts finite numeric arrays.

## Complexity

**O(n)** time and **O(1)** extra memory for all languages.
No ML library or external numerical dependency is required.

## Implementations

| Language | Directory |
| --- | --- |
| C | [jensen_shannon_divergence_c](jensen_shannon_divergence_c/) |
| Python | [jensen_shannon_divergence_python](jensen_shannon_divergence_python/) |
| Rust | [jensen_shannon_divergence_rust](jensen_shannon_divergence_rust/) |
| R | [jensen_shannon_divergence_r](jensen_shannon_divergence_r/) |

Run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests exercise identity, disjoint support, partial overlap,
single-entry distributions, zeros, invalid probabilities, and
mismatched or non-normalized vectors.

## Limitations

Discrete aligned categorical outcomes only: both vectors must
represent the **same categories in the same order**. Does not
estimate probabilities from samples, compare arbitrary
continuous densities or calculate optimal transport.
Numerical tolerance is not a substitute for data preprocessing.
No CUDA code or device performance claims.

## References

Lin, J. (1991). Divergence measures based on the Shannon entropy.
*IEEE Transactions on Information Theory, 37*(1), 145–151.
https://doi.org/10.1109/18.61115
