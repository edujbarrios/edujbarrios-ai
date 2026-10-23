# Dense Neural Layer Forward Pass (ReLU)

## Why it belongs in the AI hub

A dense neural network layer combines a learned **affine transformation**
with a **nonlinear activation**, the fundamental building block behind
multilayer perceptrons. This reference ties together the repository's
matrix arithmetic and ReLU primitives, making their composition explicit
without requiring machine-learning frameworks (Goodfellow et al., 2016).

## Mathematical definition

Given an input vector `x` of length `input_dim`, a weight matrix
`W` with **output_dim rows and input_dim columns**, and biases `b`
of length `output_dim`:

```text
z[j] = sum(W[j, i] * x[i], i = 0..input_dim-1) + b[j]
output[j] = max(0, z[j])
```

For input `x=[2,-1]`, weights `[[1,1],[-1,2]]` and biases `[0,1]`,
the raw affine output is `[1,-3]`; after ReLU, it is `[1,0]`.

## Contract

- `input_dim >= 1`, `output_dim >= 1`, with a finite real input vector,
  rectangular weight matrix and exactly one finite bias per output neuron.
- Return a vector of length `output_dim` with nonnegative finite entries.
- Any malformed matrix, wrong dimension, NaN, infinity, non-finite product,
  running sum or post-bias value is an error. **ReLU must not hide overflow.**
- C takes the input dimension, output dimension, a flattened **row-major**
  weights array and a caller-owned output vector. The caller guarantees
  allocation of `input_dim`, `output_dim * input_dim`,
  `output_dim` and `output_dim` entries for input, weights, bias and
  result; output must not overlap inputs. C returns `bool`, writing the
  output **only on complete success**.
- Rust also uses row-major weights and returns `Result<Vec<f64>, ...>`.
  Python accepts nested rows and raises `ValueError` for errors.
  R accepts a numeric matrix with rows corresponding to output neurons
  and raises errors for invalid input.
- No batch dimension or training data are involved. This is **forward
  inference only**, without gradients or parameter fitting.
- Summation is performed in input order; floating-point rounding may differ
  slightly between language implementations and compiler platforms.

## Complexity

**O(input_dim × output_dim)** time and **O(output_dim)** result storage.
The C implementation also allocates O(output_dim) scratch space to keep
the caller's output unchanged if a later neuron fails. Python validates
and copies the full weight matrix, requiring **O(input_dim × output_dim)**
additional storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [dense_forward_c](dense_forward_c/) |
| Python | [dense_forward_python](dense_forward_python/) |
| Rust | [dense_forward_rust](dense_forward_rust/) |
| R | [dense_forward_r](dense_forward_r/) |

## Examples and tests

```text
dense_forward([2,-1], [[1,1],[-1,2]], [0,1]) -> [1,0]
dense_forward([1], [[3]], [2])              -> [5]
dense_forward([-1], [[3]], [2])             -> [0]
```

Run from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The tests also cover mismatched dimensions, non-finite weights, biases,
and overflow in multiplication and aggregation.

## Limitations

No batching, backpropagation, autograd, optimized BLAS, convolution,
attention, dropout, GPU kernel, fused activation, sparse weights or
learned parameters. The output is a reference result, not a throughput
benchmark. A GPU version should be a separately tested contribution
with representative tensor sizes and NVIDIA hardware verification.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*.
MIT Press. https://www.deeplearningbook.org/
