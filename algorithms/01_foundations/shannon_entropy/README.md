# Shannon Entropy (Base 2)

## AI motivation

**Shannon entropy** measures the uncertainty of a categorical
probability distribution. It is a fundamental building block of
information theory, decision trees, language modeling, and the analysis
of predictive uncertainty (Shannon, 1948). It complements the
catalogue's Jensen–Shannon divergence and cross-entropy loss.

## Mathematical definition

For a categorical probability vector `p=(p_0,...,p_{n-1})`:

```text
H_2(p) = -sum_i p_i * log2(p_i)
```

The contribution of **p_i=0** is defined as **0**, so `log2(0)`
is never evaluated. Base 2 expresses information in **bits**;
natural logarithms would express it in nats.

Examples:

```text
p=[1,0]                 -> H_2=0 bits
p=[0.5,0.5]             -> H_2=1 bit
p=[0.25,0.25,0.25,0.25] -> H_2=2 bits
p=[0.75,0.25]           -> H_2≈0.811278124459 bits
```

The minimum is 0 for a point mass. The maximum is `log2(n)`
for a uniform distribution over n outcomes.

## Shared contract

- Accept one **nonempty** vector with **1<=n<=4096**.
- Every probability must be a finite real in `[0,1]`.
  The sum must be within **absolute tolerance 1e-9** of 1.
  Input values are not silently renormalized.
- Return one finite, nonnegative entropy value measured in bits.
  Tiny negative results from rounding are clamped to zero.
- Reject empty or oversized arrays, NaN/Inf, invalid numeric types,
  negative probabilities, entries above one, non-normalized totals,
  or nonfinite accumulated information.
- C accepts `double probabilities[n]` and writes to caller-owned
  `double* entropy` only on success. Input and output buffers must
  be appropriately sized and disjoint. Rust returns
  `Result<f64,...>`; Python and R raise validation errors.
  Python rejects boolean values; R requires base numeric vectors.
- The output describes the supplied categorical distribution, not
  differential entropy of continuous densities and not the
  cross-entropy between two probability vectors.

## Complexity

**O(n)** time and **O(1)** auxiliary memory.

## Implementations

| Language | Directory |
| --- | --- |
| C | [shannon_entropy_c](shannon_entropy_c/) |
| Python | [shannon_entropy_python](shannon_entropy_python/) |
| Rust | [shannon_entropy_rust](shannon_entropy_rust/) |
| R | [shannon_entropy_r](shannon_entropy_r/) |

Run the strict tests from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The fixtures cover point masses, uniform and nonuniform
distributions, zero probabilities, malformed normalization,
invalid numeric types, and nonfinite inputs.

## Limitations

This is a finite discrete distribution statistic. It does not
estimate probabilities from samples, select a histogram binning
strategy, perform language-model training, implement information gain,
or provide a CUDA/GPU implementation. The chosen sum tolerance permits
very small distribution-normalization differences.

## References

Shannon, C. E. (1948). A mathematical theory of communication.
*The Bell System Technical Journal, 27*(3), 379–423.
https://doi.org/10.1002/j.1538-7305.1948.tb01338.x
