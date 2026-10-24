# Temperature-Scaled Softmax

## Why it belongs in AI

**Softmax** maps raw model scores (*logits*) to a probability
distribution. Applying **temperature** changes how concentrated the
distribution is: smaller positive temperatures favor the highest
logits, while larger temperatures flatten the probabilities. This
technique appears in knowledge distillation and in decoding from
language models (Hinton et al., 2015).

This reference computes probabilities only. It does **not** perform a
random draw, apply top-k/top-p filtering, or learn a temperature.

## Mathematics

For logits `z[i]` and strictly positive `T`, define

```text
p[i] = exp(z[i]/T) / sum_j exp(z[j]/T)

m = max(z)
w[i] = exp((z[i]-m)/T)
p[i] = w[i]/sum_j(w[j])
```

Subtracting the maximum logit before exponentiation prevents
overflow for ordinary large logits. Very large differences or
division by extremely small T can still overflow; this reference
rejects such inputs instead of silently making an invalid distribution.

Examples:

```text
logits=[0,ln(3)], temperature=1
probabilities=[0.25,0.75]

logits=[0,ln(3)], temperature=0.5
probabilities=[0.1,0.9]

logits=[0,ln(3)], temperature=2
probabilities=[1/(1+sqrt(3)),sqrt(3)/(1+sqrt(3))]

logits=[10,10], temperature=1 -> [0.5,0.5]
```

## Shared contract

- Logits must be a **nonempty** finite real vector of length
  **1..4096**. T must be a finite real **strictly > 0**.
- Return a finite vector of nonnegative probabilities of the same
  length, with total approximately 1 (subject to floating-point
  rounding). Input values can be negative, zero or positive.
- Reject NaN/infinity, invalid types, empty or oversized vectors,
  invalid temperature, nonfinite max-subtracted differences,
  scaled logits, exponential weights, sum or normalized outputs.
- C takes `double logits[n]` and caller-owned
  `double probabilities[n]` with no overlap; failure leaves the
  entire output buffer unchanged. Rust returns `Result<Vec<f64>,...>`;
  Python/R raise errors. Python does not accept boolean numeric values.
- `T=1` produces ordinary softmax. For small T, very low scores
  may underflow to probability zero, which is valid.

## Complexity

O(n) time and O(n) storage for n probabilities. C uses a temporary
buffer to guarantee atomic output on failure.

## Implementations

| Language | Directory |
| --- | --- |
| C | [temperature_softmax_c](temperature_softmax_c/) |
| Python | [temperature_softmax_python](temperature_softmax_python/) |
| Rust | [temperature_softmax_rust](temperature_softmax_rust/) |
| R | [temperature_softmax_r](temperature_softmax_r/) |

Test from the root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover temperature 0.5, 1, 2, equal logits, extreme common
offsets, invalid temperature, NaN/Inf and unrepresentable scaling.

## Limitations

A temperature is **not a guarantee of calibration**; fitting it to
held-out data and assessing forecast calibration is separate.
No learnable temperature, categorical sampling, backpropagation,
tensor broadcasting, mixed precision or CUDA performance claims.

## References

Hinton, G., Vinyals, O., & Dean, J. (2015). *Distilling the
knowledge in a neural network* [Preprint]. arXiv.
https://arxiv.org/abs/1503.02531
