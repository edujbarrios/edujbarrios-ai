# Softplus Activation

## Why it belongs in this repository

Softplus is a smooth approximation of ReLU used as a neural-network activation.
Its derivative is the sigmoid function (Nair & Hinton, 2010). It is also
useful for converting an unconstrained model score into a nonnegative scale.

## Mathematical definition

For finite x:

    softplus(x) = log(1 + exp(x))
    derivative(x) = 1 / (1 + exp(-x))

The stable formula is max(x, 0) + log1p(exp(-abs(x))); the derivative uses
separate exponential branches for nonnegative and negative x.

## Contract

- Input: a nonempty one-dimensional vector of finite real values.
- Output: ordered pairs of vectors (values, derivatives), with the same length
  as the input.
- Python raises ValueError for empty, Boolean, nonnumeric and nonfinite
  inputs. R throws an error for empty, nonreal and nonfinite inputs.
- Rust returns Err on empty or nonfinite input. C returns false on null
  pointers, zero length or nonfinite values without writing to outputs.
  C input and output buffers must be distinct and correctly sized.
- For very negative x, the result and gradient may underflow to exactly 0.
  For large positive x, the result approaches x and the gradient approaches 1.

## Complexity

O(n) time and O(n) result memory for n elements; the C variant uses two
caller-provided output buffers and allocates no extra heap memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [softplus_c](softplus_c/) |
| Python | [softplus_python](softplus_python/) |
| Rust | [softplus_rust](softplus_rust/) |
| R | [softplus_r](softplus_r/) |

## Examples and tests

    softplus([0, 1000, -1000])
    values      = [ln(2), 1000, 0]
    derivatives = [0.5, 1, 0]

From the repository root, run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

The tests cover known values, extreme magnitudes, empty input and
nonfinite inputs.

## Limitations

Educational scalar/vector implementation only, not an optimized tensor or
GPU kernel. It does not backpropagate through a full network or implement a
parameterized temperature. Underflow at large negative inputs is expected.

## References

Nair, V., & Hinton, G. E. (2010). Rectified linear units improve restricted
Boltzmann machines. In *Proceedings of the 27th International Conference
on Machine Learning* (pp. 807–814).
https://icml.cc/2010/papers/432.pdf
