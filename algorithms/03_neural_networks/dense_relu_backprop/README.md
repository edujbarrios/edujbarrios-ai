# Dense ReLU Layer Backpropagation

## Why it belongs here

Backpropagation applies the chain rule to distribute a loss gradient from
one neural network layer to the parameters and inputs that produced its
output. This algorithm computes the **local backward pass** for one fully
connected layer followed by ReLU, complementing the repository's
`dense_forward` forward-pass reference (Goodfellow et al., 2016).

## Mathematical definition

Given an input vector `x` of dimension `d`, a dense weight matrix `W`
of shape `o × d`, biases `b` of length `o`, and an upstream gradient
`g[j] = dL/dy[j]` (for the ReLU-activated output), define:

```text
z[j] = sum(W[j,i] * x[i], i=0..d-1) + b[j]
dz[j] = g[j] if z[j] > 0 else 0

dL/db[j]     = dz[j]
dL/dW[j,i]   = dz[j] * x[i]
dL/dx[i]     = sum(W[j,i] * dz[j], j=0..o-1)
```

At the non-differentiable ReLU boundary `z=0`, **the subgradient is
defined here as zero**. This fixed convention makes results reproducible
and matches a common training choice. This function performs no optimizer
update; it only computes gradients.

Example:

```text
x = [2,-1]
W = [[1,1],[-1,2]]
b = [0,1]
upstream = [3,4]

z = [1,-3]
dz = [3,0]
grad_input   = [3,3]
grad_weights = [[6,-3],[0,0]]
grad_bias    = [3,0]
```

## Contract

- All input dimensions are **strictly positive**: `d>=1`, `o>=1`.
- The input vector has length `d`, weights shape `o × d`, and bias and
  upstream gradient each length `o`. All components must be finite real
  values; C/Rust use flattened **row-major** weights.
- Return three gradient objects: `grad_input[d]`,
  `grad_weights[o,d]`, `grad_bias[o]` in that order.
- Reject NaN, infinity, shape mismatch, non-finite forward values,
  gradient products or sums. ReLU masking **must not silently conceal**
  non-finite affine computations; the entire forward affine step is checked
  before deciding if a neuron is active.
- C accepts caller-allocated output buffers of the correct sizes and
  must not alias its inputs. It commits output only after successful
  full computation. Python/R raise errors; Rust returns
  `Result<(Vec<f64>, Vec<f64>, Vec<f64>), ...>`.
- Floating-point summation occurs in neuron/feature iteration order and
  may differ slightly from optimized matrix implementations.
- The caller is responsible for supplying an upstream gradient for a
  **scalar loss**; this function does not derive a specific loss.
- This is a local backward pass only. It does not train a complete network,
  update parameters, or perform autodifferentiation across many layers.

## Complexity

**O(o × d)** time and **O(o × d + o + d)** output storage. C allocates
scratch gradients to preserve caller-provided outputs on failure.
Python additionally validates/copies the `o × d` weight matrix.

## Implementations

| Language | Folder |
| --- | --- |
| C | [dense_relu_backprop_c](dense_relu_backprop_c/) |
| Python | [dense_relu_backprop_python](dense_relu_backprop_python/) |
| Rust | [dense_relu_backprop_rust](dense_relu_backprop_rust/) |
| R | [dense_relu_backprop_r](dense_relu_backprop_r/) |

## Examples and tests

```text
backprop([2,-1], [[1,1],[-1,2]], [0,1], [3,4])
-> grad_input=[3,3], grad_weights=[[6,-3],[0,0]], grad_bias=[3,0]

backprop([0], [[3]], [0], [5])
-> grad_input=[0], grad_weights=[[0]], grad_bias=[0]
   (subgradient at z=0 is zero)
```

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` at the repository root.
Tests cover active/inactive ReLU nodes, derivative at zero, one-neuron
behavior, shape mismatch and numeric overflow.

## Limitations

No minibatches, optimizer updates, activation derivatives other than ReLU,
dropout, non-affine layers, automatic differentiation or CUDA acceleration.
No GPU speedup should be asserted without NVIDIA hardware benchmarks.
This deliberately omits trainable recurrent/attention architectures.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*.
MIT Press. https://www.deeplearningbook.org/
