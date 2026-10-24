# Swish (SiLU) Activation and Derivative

## Why it belongs in AI

Swish is a smooth neural-network activation defined by multiplying an input
by its sigmoid. Also called the **sigmoid linear unit (SiLU)**, it differs
from ReLU because it permits small negative outputs and is not monotonic.
Its analytical derivative supports training by backpropagation. Ramachandran
et al. (2017) investigated it in the context of activation-function search.

## Mathematics

For each real input `x`:

```text
s = sigmoid(x) = 1 / (1 + exp(-x))
swish(x) = x * s
swish'(x) = s + x * s * (1 - s)
```

The implementation evaluates sigmoid with a numerically stable branch:

```text
if x >= 0: s = 1 / (1 + exp(-x))
if x < 0:  z = exp(x); s = z / (1 + z)
```

This avoids evaluating `exp(-x)` on a very negative value. In particular,
`swish(0) = 0` and `swish'(0) = 0.5`. For large positive inputs
the output approaches the input, and for large negative inputs it
approaches zero from below.

## Shared contract

- Accept a **nonempty**, one-dimensional vector of **finite real values**.
- Return two vectors of equal length: elementwise activations and analytical
  derivatives, in the original order. R returns named elements `values`
  and `derivatives`; Python/Rust return a pair of arrays.
- Reject NaN, infinity, invalid types or empty vectors. All intermediate
  calculations and outputs must remain finite; finite very large values
  are allowed and processed through the stable sigmoid branches.
- C receives `double x[n]`, caller-owned `values[n]` and
  `derivatives[n]` buffers (all non-overlapping), and returns `bool`.
  Outputs are unchanged if validation or allocation fails.
  Rust returns `Result<(Vec<f64>, Vec<f64>), ...>`. Python/R raise errors.
- Floating-point results are approximate; exact negative zero may arise
  for very negative inputs and is numerically equal to positive zero.

## Complexity

O(n) time and O(n) output memory. The C implementation uses O(n) temporary
storage to make error handling atomic; Rust/Python/R allocate output
vectors. No ML, vectorization or GPU dependency is required.

## Implementations

| Language | Directory |
| --- | --- |
| C | [swish_c](swish_c/) |
| Python | [swish_python](swish_python/) |
| Rust | [swish_rust](swish_rust/) |
| R | [swish_r](swish_r/) |

## Examples and tests

```text
swish([0, 1, -1])
values      = [0, 0.7310585786, -0.2689414214]
derivatives = [0.5, 0.9276705119, 0.0723294881]
```

From the repository root, run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover a known numerical fixture, zero, positive/negative extremes,
invalid nonfinite inputs and zero-length vectors.

## Limitations

This is an elementwise CPU reference, not an automatic-differentiation
engine, a neural layer, fused gradient kernel or full training loop.
The derivative is with respect to **x**, not a learnable parameter.
No CUDA claims or benchmarks are made without GPU-specific testing.

## References

Ramachandran, P., Zoph, B., & Le, Q. V. (2017). *Searching for activation
functions* [Preprint]. arXiv. https://arxiv.org/abs/1710.05941
