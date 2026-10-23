# Numerically Stable Sigmoid

**Stage:** Neural Networks · **Languages:** C, Python, Rust and R.

## Why it belongs here

The logistic sigmoid transforms real-valued scores into a range between
zero and one. It appears in binary classification, logistic regression and
gating mechanisms. Its derivative and saturation also help explain
training difficulties in deep networks (Goodfellow et al., 2016).

## Algorithm

For a real scalar `x`, `sigmoid(x)=1/(1+exp(-x))`. To avoid overflow
when `x` is very negative, use equivalent numerically stable branches:

```text
if x >= 0: 1 / (1 + exp(-x))
else:      exp(x) / (1 + exp(x))
```

Applied elementwise: `[-1, 0, 1]` maps approximately to
`[0.268941, 0.5, 0.731059]`.

## Contract

- Input: dense vector of finite real numbers, including an empty vector.
- Output: a vector of equal size with finite values in `[0,1]`.
  Extremely large positive or negative inputs can round to exactly 1 or 0.
- Reject NaN, infinity and nonnumeric types. For valid finite inputs, no
  exponentiation overflows in the stable-branch implementation.
- C returns `bool`, accepts exact in-place operation, requires
  caller-owned buffers of size `n`, and performs full validation before
  writing to output; output is unchanged on invalid input.
- Python/R raise errors; Rust returns `Result<Vec<f64>,...>`.

## Complexity

**O(n)** work. C uses O(1) auxiliary memory, while Python/Rust/R return
O(n) output vectors.

## Implementations

| Language | Folder |
| --- | --- |
| C | [sigmoid_c](sigmoid_c/) |
| Python | [sigmoid_python](sigmoid_python/) |
| Rust | [sigmoid_rust](sigmoid_rust/) |
| R | [sigmoid_r](sigmoid_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover zero, sign symmetry, monotonicity, extreme finite logits, empty
input, non-finite elements and in-place C output.

## Limitations

Only the elementwise forward activation. No derivative, loss function,
logistic regression trainer, broadcasting, batch semantics or GPU kernels.
The scalar values are not calibrated probabilities without a fitting model.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*.
MIT Press. https://www.deeplearningbook.org/
