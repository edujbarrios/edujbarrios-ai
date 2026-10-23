# Leaky Rectified Linear Unit (Leaky ReLU)

## Why it belongs here

Leaky ReLU illustrates how a small change to ReLU permits a nonzero slope
on negative inputs. It is useful for comparing nonlinear activations and
understanding gradient flow in neural networks. The activation is described
in the work of Maas et al. (2013).

## Mathematical definition

For an input `x` and a slope `alpha` satisfying `0 < alpha < 1`:

```text
leaky_relu(x, alpha) = x       if x >= 0
                       alpha*x if x < 0
```

Example: `leaky_relu([-2, 0, 3], 0.1) == [-0.2, 0, 3]`.

## Contract and edge cases

- Accept finite real vectors, including the empty vector, and a finite slope
  strictly between zero and one. Each output has the same dimension.
- Reject NaN, infinity, booleans-as-numbers, and invalid slopes. Python and R
  raise errors; Rust returns `Result<Vec<f64>, ...>`; C returns `bool`.
- C requires valid caller-provided input and output buffers with `n`
  elements, allows an identical in-place buffer, and leaves the output
  unchanged if validation fails. With `n=0`, a null input is permitted.
- This implements the forward activation, **not** a gradient at zero.
  At `x=0` a derivative/subgradient policy must be chosen separately.
- Finite double inputs and `0<alpha<1` avoid overflow in negative products,
  although very small negative outputs can underflow to negative zero.

## Complexity

**O(n)** time. C uses O(1) auxiliary memory; Python, Rust, and R allocate an
O(n) output. There are no third-party dependencies.

## Implementations

| Language | Source directory |
| --- | --- |
| C | [leaky_relu_c/](leaky_relu_c/) |
| Python | [leaky_relu_python/](leaky_relu_python/) |
| Rust | [leaky_relu_rust/](leaky_relu_rust/) |
| R | [leaky_relu_r/](leaky_relu_r/) |

CUDA is not included; a GPU version should be contributed with device tests
and batch-size benchmarks rather than assuming the GPU is faster.

## Tests

Run `python3 scripts/run_tests.py --strict` at the repository root. Suites
check positive, negative, zero and empty vectors, in-place C behavior,
invalid alpha, and non-finite values.

## Limitations

No trainable PReLU parameter, tensor broadcasting, autograd, or fused GPU
kernels. This is a reference implementation, not a performance claim.

## References

Maas, A. L., Hannun, A. Y., & Ng, A. Y. (2013). Rectifier nonlinearities
improve neural network acoustic models. In *Proceedings of the 30th
International Conference on Machine Learning Workshop on Deep Learning for
Audio, Speech, and Language Processing*.
https://ai.stanford.edu/~amaas/papers/relu_hybrid_icml2013_final.pdf
