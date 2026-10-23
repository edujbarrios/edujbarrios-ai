# Batch Gradient Descent for Linear Regression

**Stage:** Classical Machine Learning · **Languages:** C, Python, Rust, R.

## Why it belongs here

Gradient descent trains models iteratively by moving parameters against the
loss gradient. This full-batch example trains a one-feature linear model
without automatic differentiation or ML frameworks. It contrasts with the
closed-form OLS algorithm already in the catalogue and introduces optimizers
used throughout neural-network training (Ruder, 2016).

## Algorithm

Given finite pairs `(x_i, y_i)`, fit `y_hat_i = weight * x_i + bias`
with the mean squared error objective:

```text
L(weight, bias) = (1/n) * sum((weight*x_i + bias - y_i)²)
dL/dweight = (2/n) * sum((weight*x_i + bias - y_i) * x_i)
dL/dbias   = (2/n) * sum(weight*x_i + bias - y_i)
weight' = weight - learning_rate * dL/dweight
bias'   = bias   - learning_rate * dL/dbias
```

Start at `weight = bias = 0` and execute exactly `epochs` full-data
updates. For `x=[0,1,2,3]`, `y=[1,3,5,7]`, learning rate `0.05` and
`2000` epochs, parameters approach `(2, 1)`.

## Contract

- `x`, `y`: two same-length, **nonempty** sequences of finite real values.
- `learning_rate`: finite and strictly positive; `epochs`: positive integer.
- Output: `(weight, bias)`; no stochasticity or shuffling.
- Abort on non-finite predictions, residuals, gradient sums or parameter
  updates. An excessively large learning rate can diverge and cause an error;
  **finite output alone is not a convergence guarantee**.
- C takes two arrays with caller-verified lengths and distinct output
  pointers. On error, output pointers remain unchanged. Python/R raise
  errors; Rust returns `Result<(f64,f64), ...>`.
- Data are not normalized automatically. Users should scale features and
  choose learning rates suited to their data.

## Complexity

**O(epochs × n)** time and **O(1)** auxiliary memory in all four
implementations (excluding the original input arrays).

## Implementations

| Language | Folder |
| --- | --- |
| C | [gradient_descent_c](gradient_descent_c/) |
| Python | [gradient_descent_python](gradient_descent_python/) |
| Rust | [gradient_descent_rust](gradient_descent_rust/) |
| R | [gradient_descent_r](gradient_descent_r/) |

CUDA is not part of this baseline; an accelerated batched optimizer must be
a separate, tested and benchmarked contribution.

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests verify convergence toward the known `y=2x+1` parameters, a
constant-target baseline, and rejection of missing/non-finite values,
invalid learning rates or epoch counts.

## Limitations

A single input feature, full-batch squared loss, fixed learning rate and zero
initialization only. No momentum, adaptive step size, regularization,
mini-batches, train/validation split, automatic stopping or convergence
proof for arbitrary data.

## References

Ruder, S. (2016). *An overview of gradient descent optimization algorithms*
(arXiv:1609.04747). arXiv. https://doi.org/10.48550/arXiv.1609.04747
