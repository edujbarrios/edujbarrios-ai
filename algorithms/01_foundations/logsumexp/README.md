# Stable Log-Sum-Exp and Gradient

## Why it belongs in AI

Many probabilistic algorithms work in log space to avoid multiplying
small probabilities that would underflow to zero. To combine these
paths correctly, models must evaluate **log-sum-exp** (LSE), not
just the maximum of the log scores. It appears in partition
functions, cross-entropy, Softmax, Hidden Markov Model inference,
and dynamic programming (Bishop, 2006).

The **gradient of log-sum-exp is Softmax**, which also makes this
small algorithm a useful numerical and calculus reference.

## Mathematics

For nonempty real vector `x`:

```text
LSE(x) = ln(sum_i exp(x[i]))

m = max_i(x[i])
z[i] = exp(x[i] - m)
LSE(x) = m + ln(sum_i z[i])
d LSE(x) / dx[i] = z[i] / sum_j z[j]
```

Shifting each coordinate by `m` before exponentiation avoids
overflow for ordinary large positive logits. The gradient forms
a finite, nonnegative vector summing to approximately 1.

Examples:

```text
x=[0,0]          LSE=ln(2), gradient=[0.5,0.5]
x=[ln(2),ln(3)]  LSE=ln(5), gradient=[0.4,0.6]
x=[-1000,-1000]  LSE=-1000+ln(2), gradient=[0.5,0.5]
x=[5]            LSE=5, gradient=[1]
```

## Shared contract

- Input: a **nonempty 1..4096** vector of **finite real** values.
- Output: a finite real scalar log-sum-exp and an aligned vector of
  finite nonnegative gradient entries, approximately summing to 1.
- Reject malformed, NaN/Inf or unrepresentable numeric values.
  Although subtracting the maximum avoids ordinary exponent
  overflow, extreme finite input ranges can overflow
  `x[i]-m`; the implementation **rejects** those cases rather
  than silently treating them as negative infinity.
- Intermediate exponential weights, weight sums, final result and
  gradient entries must all be finite. Very negative shifted values
  may legitimately underflow to weight zero.
- C reads `double x[n]`, writes caller-owned `double* value`
  and `double gradient[n]` **only after the entire computation
  succeeds**; output and input buffers must not overlap.
  Rust returns `Result<(f64,Vec<f64>),...>`; Python returns
  `(value, gradients)`; R returns `list(value=...,gradient=...)`.
- Python rejects bool-as-numeric values. Minor cross-language
  rounding differences are possible.

## Complexity

**O(n)** time and O(n) output storage, plus C scratch space for
atomic outputs. Only standard math libraries are needed.

## Implementations

| Language | Directory |
| --- | --- |
| C | [logsumexp_c](logsumexp_c/) |
| Python | [logsumexp_python](logsumexp_python/) |
| Rust | [logsumexp_rust](logsumexp_rust/) |
| R | [logsumexp_r](logsumexp_r/) |

Run all reference suites:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover equal logits, unequal logits, negative large offsets,
single-element inputs, invalid input and unrepresentable differences.

## Limitations

No batching, `-Inf` sentinel inputs (sometimes used for impossible
events), weighted sums, signed log sums, GPU kernels or framework
automatic differentiation. The strict finite-only contract is
intentional: HMM Forward separately implements the two-term
log-sum-exp needed for its own probability semantics.

## References

Bishop, C. M. (2006). *Pattern recognition and machine learning*.
Springer.
