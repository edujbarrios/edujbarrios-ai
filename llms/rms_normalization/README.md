# Root Mean Square Normalization (RMSNorm)

## LLM context

RMSNorm is a normalization operation widely used in transformer
language models. Unlike LayerNorm, RMSNorm **does not subtract
the feature mean**; it rescales by the root mean square and then
applies a feature-wise gain (Zhang & Sennrich, 2019).

## Formula

Given token features `x[i]`, learned-style gains `g[i]` and
a small positive `epsilon`:

```text
rms = sqrt((1/D) * sum(x[i]^2) + epsilon)
y[i] = (x[i]/rms)*g[i]
```

For `x=[1,1]`, `g=[1,2]` and `epsilon=1`,
`y=[1/sqrt(2),2/sqrt(2)]`. For zero features the output
is all zero. No trainable state is updated.

## Contract

- One token feature vector `x` and gain vector `g`, each of
  exactly the same length `D=1..1024`.
- Each element must be finite and in `[-1000,1000]`.
- `epsilon` must be finite in `[1e-8,1]`.
- Output is a `D`-element vector of finite real numbers.
- C requires caller-allocated, separate `D`-element output
  which is left unchanged on validation failure. Python rejects
  boolean numeric values; Rust returns `Result<Vec<f64>,&str>`
  and R returns a numeric vector.
- No batch, gradients, weight updates, bias or layer averaging.

## Complexity

**O(D)** time and **O(D)** output space, with constant scratch in C.

## Implementations

| Language | Reference |
| --- | --- |
| C | [rms_normalization_c](rms_normalization_c/) |
| Python | [rms_normalization_python](rms_normalization_python/) |
| Rust | [rms_normalization_rust](rms_normalization_rust/) |
| R | [rms_normalization_r](rms_normalization_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover nonconstant and constant vectors, zero features,
gain scaling, epsilon bounds and nonfinite or malformed inputs.

## Limitations

This single-token implementation is not a full LLM layer or
fused GPU implementation. The educational epsilon range may differ
from model configurations. It does not learn `g` or provide
automatic differentiation.

## References

- Zhang, B., & Sennrich, R. (2019). *Root mean square layer normalization*. Advances in Neural Information Processing Systems, 32. https://arxiv.org/abs/1910.07467
