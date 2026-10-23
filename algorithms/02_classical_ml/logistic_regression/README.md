# Binary Logistic Regression

## Why it belongs in this repository

Logistic regression is a foundational probabilistic **binary classifier**.
It maps a linear score to a probability using the sigmoid and learns
parameters by reducing binary cross-entropy, connecting classic supervised
learning to neural-network training (Hastie et al., 2009). This reference
implements **one numeric input feature** using deterministic full-batch
gradient descent, without external ML or numerical libraries.

## Model and algorithm

Given paired observations `(x_i, y_i)` with binary `y_i ∈ {0,1}`:

```text
z_i = weight * x_i + bias
p_i = 1 / (1 + exp(-z_i))
BCE = -mean(y_i * log(p_i) + (1 - y_i) * log(1 - p_i))

gradient_weight = mean((p_i - y_i) * x_i)
gradient_bias   = mean(p_i - y_i)
weight -= learning_rate * gradient_weight
bias   -= learning_rate * gradient_bias
```

We initialize `weight = bias = 0`, perform **exactly** the requested
number of epochs, and return the fitted `(weight, bias)` pair. The
sigmoid is implemented with a **piecewise exponential** that avoids
unnecessary overflow at large finite logits.

For `x=[0,1,2,3]`, `y=[0,0,1,1]`, learning rate `0.2` and `2000`
epochs, the learned model assigns a low probability to `x=0` and a
high probability to `x=3`.

## Shared contract

- Inputs: equal-size **nonempty** arrays of finite real `x` and binary
  labels `y` exactly 0 or 1. Both classes are not required for a valid run.
- `learning_rate` must be finite and strictly positive; `epochs` must be
  a **positive integer**. No random seed or shuffling is involved.
- Output: finite `(weight, bias)`. Callers can compute a probability from
  them using the model formula and the stable sigmoid.
- Invalid labels, dimensions, parameters, non-finite intermediate logits,
  gradients or parameter updates cause an error. A large learning rate can
  diverge; **the returned parameters do not guarantee convergence**.
- C accepts arrays of at least `n` elements and distinct non-null output
  pointers. It returns `bool` and does not modify outputs on failure.
  Rust returns `Result<(f64, f64), ...>`; Python and R raise errors.
- Floating-point convergence is approximate. Tests compare probabilities
  rather than asserting exact fitted coefficients across toolchains.

## Complexity

For `E` training epochs and `n` observations, **O(E × n)** time and
**O(1)** auxiliary memory in C, Rust and R. The Python reference converts
inputs to float lists and therefore uses **O(n)** auxiliary memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [logistic_regression_c](logistic_regression_c/) |
| Python | [logistic_regression_python](logistic_regression_python/) |
| Rust | [logistic_regression_rust](logistic_regression_rust/) |
| R | [logistic_regression_r](logistic_regression_r/) |

## Examples and tests

```text
train([0,1,2,3], [0,0,1,1], learning_rate=0.2, epochs=2000)
-> a positive weight and probabilities p(0) < 0.1, p(3) > 0.9

train([1], [1], learning_rate=0.2, epochs=1)
-> finite weight and bias
```

Run from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

Single predictor only: no multivariate features, mini-batches, class weights,
L1/L2 regularization, feature scaling, intercept constraints, convergence
stopping criteria or calibrated confidence intervals. Large or unscaled
features can cause poorly conditioned optimization. This is not a replacement
for production-quality statistical software.

CUDA is deliberately omitted until a GPU implementation has reproducible
device testing and benchmarks for a meaningfully large workload.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of
statistical learning: Data mining, inference, and prediction* (2nd ed.).
Springer. https://doi.org/10.1007/978-0-387-84858-7
