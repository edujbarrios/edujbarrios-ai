# Rectified Linear Unit (ReLU)

**Stage:** Neural Networks · **Languages:** C, Python, Rust and R.

## Why it belongs here

Nonlinear activation functions let neural networks represent transformations
that cannot be expressed by simply stacking linear layers. ReLU is a
widely used, computationally simple example. Nair and Hinton (2010) discussed
rectified linear units in connection with restricted Boltzmann machines.

## What it does

Apply the activation independently to each input:

```text
ReLU(x) = max(0, x)

input  = [-2, -0.5, 0, 3]
output = [ 0,  0,   0, 3]
```

The mathematical derivative is zero for negative inputs and one for positive
inputs; it is **not uniquely defined at x=0**. This block implements only
the forward pass, not a derivative or a training subgradient convention.

## Shared contract

- Input: a dense vector of finite real numbers; **empty vectors are valid**
  and produce an empty output.
- Output: an equally sized vector; negative values become zero; positive
  values are preserved, including their finite magnitude.
- NaN and infinity are invalid. No output is returned on error.
- C receives caller-owned input and output arrays of at least `n`
  elements, allows exact in-place updates, and returns `bool`. C first
  validates the full input so an error cannot partially modify output.
  Empty C arrays may be null if `n=0`, but `out` must be non-null.
- Python/R raise an error. Rust returns `Result<Vec<f64>, ...>`.

## Complexity

**O(n)** time. The C version uses **O(1)** additional space besides its
caller-owned output. Python, Rust and R allocate **O(n)** for the result.

## Implementations

| Language | Directory |
| --- | --- |
| C | [relu_c](relu_c/) |
| Python | [relu_python](relu_python/) |
| Rust | [relu_rust](relu_rust/) |
| R | [relu_r](relu_r/) |

An elementwise CUDA kernel could be added later with GPU hardware tests.
A single tiny vector does not justify claims of GPU speedup.

## Run tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

Dense vectors only; no tensor shapes, batching, automatic differentiation,
learned activation parameters, or deep-learning framework dependencies.
The forward function says nothing about gradient choice at the origin.

## References

Nair, V., & Hinton, G. E. (2010). Rectified linear units improve
restricted Boltzmann machines. In *Proceedings of the 27th International
Conference on Machine Learning* (pp. 807–814).
https://icml.cc/2010/papers/432.pdf
