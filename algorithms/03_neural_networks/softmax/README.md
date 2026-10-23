# Numerically Stable Softmax

**Stage:** Neural Networks · **Languages:** C, Python, Rust, R.

## Why this algorithm belongs here

Softmax converts a vector of unnormalized scores (logits) into a probability
distribution. It is commonly used in multiclass classification and attention.
Subtracting the largest logit prevents overflow that would occur with direct
exponentials (Goodfellow et al., 2016).

## Algorithm and example

For finite, nonempty logits `x`:

```text
m = max(x)
softmax(x)[i] = exp(x[i] - m) / sum_j exp(x[j] - m)
```

For `[1, 1, 1]`, the output is `[1/3, 1/3, 1/3]`.
For `[1000, 1001]`, subtraction of 1001 produces finite exponentials.
The elements are nonnegative and sum to approximately one.

## Contract

- Input: a **nonempty** vector of finite real logits.
- Output: an equally sized vector of finite probabilities in [0,1], whose
  total is approximately 1. Negative extreme differences may underflow
  to zero; this is expected IEEE-754 behavior.
- Reject empty vectors, nonnumeric values, NaN or infinity.
- C requires valid input/output buffers of at least `n` elements; exact
  in-place output is supported. A non-finite value causes an error without
  modifying output. Python/R raise an error; Rust returns `Result<Vec<f64>,...>`.
- This is one distribution, **not** a batched tensor or a hard class decision.

## Complexity

**O(n)** time. C uses O(1) auxiliary memory besides the caller-owned
output. Python/Rust/R return or create O(n) output storage.

## Implementations

| Language | Directory |
| --- | --- |
| C | [softmax_c](softmax_c/) |
| Python | [softmax_python](softmax_python/) |
| Rust | [softmax_rust](softmax_rust/) |
| R | [softmax_r](softmax_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Test fixtures cover equal logits, extreme but finite logits,
monotonic probabilities, singleton vectors and non-finite input.

## Limitations

No tensor batching, temperature parameter, masks, automatic differentiation,
sparse softmax, arbitrary-precision arithmetic or GPU kernel. A CUDA version
should be contributed with GPU-specific tests and benchmarks.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*.
MIT Press. https://www.deeplearningbook.org/
