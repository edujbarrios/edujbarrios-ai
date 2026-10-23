# Mini-Batch Gradient Descent (Univariate Linear Regression)

## Why it belongs in the AI hub

Mini-batch gradient descent connects full-batch gradient descent and
per-sample stochastic gradient descent (SGD). It estimates a loss gradient
from consecutive groups of training samples, updating the model after
each group. This training pattern is fundamental to scalable supervised
learning (Bottou, 2010). Here it trains a simple linear predictor without
third-party numerical libraries.

## Definition

Given finite feature/target pairs `(x_i, y_i)`, model
`prediction = weight*x_i + bias` and minimize half-mean-squared-error
`L = (1/(2n))*sum((prediction-y_i)^2)`.

Initialize `weight=bias=0`. For each epoch, process samples in their
**original order**, in batches of `batch_size`. A shorter final batch is
included rather than discarded. For each batch of size `m`:

```text
error_i = weight*x_i + bias - y_i
grad_weight = sum(error_i*x_i) / m
grad_bias   = sum(error_i) / m
weight -= learning_rate * grad_weight
bias   -= learning_rate * grad_bias
```

Updates happen **once per batch**, using the model parameters from the
start of that batch. For `x=[0,1]`, `y=[1,3]`, learning rate `0.1`,
batch size `1`, one epoch, the result is approximately
`weight=0.29, bias=0.39`, unlike one full-batch update.

## Contract

- `x` and `y` are equal-length, **nonempty** finite real vectors.
- `learning_rate` must be finite and positive. `epochs` is a positive
  integer; `batch_size` is an integer in `[1,n]`.
- Output is a finite pair `(weight,bias)`. There is **no random shuffling**:
  data order affects the result, and is part of the API.
- Exactly the requested number of epochs is performed. No early stopping,
  convergence guarantee or automatic learning-rate decay is used.
- Non-finite intermediate predictions, gradient products/sums, step updates
  or fitted parameters are errors. Very large feature values can cause
  divergence even if all original inputs are finite.
- C requires caller-owned feature/target arrays (at least n elements) and
  two distinct non-null output pointers; outputs remain unchanged on error.
  Rust returns a `Result`; Python and R raise errors.
- Neither a separate train/test split nor feature standardization is
  performed; data preparation belongs to the caller.

## Complexity

`O(epochs × n)` arithmetic time; **O(1)** auxiliary memory in C, Rust,
and R. Python copies and validates input vectors (O(n) auxiliary space).
Batch size affects the number of parameter updates, not the number of
training observations processed.

## Implementations

| Language | Folder |
| --- | --- |
| C | [mini_batch_gradient_descent_c](mini_batch_gradient_descent_c/) |
| Python | [mini_batch_gradient_descent_python](mini_batch_gradient_descent_python/) |
| Rust | [mini_batch_gradient_descent_rust](mini_batch_gradient_descent_rust/) |
| R | [mini_batch_gradient_descent_r](mini_batch_gradient_descent_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover a known one-epoch ordered update, fitting `y=2x+1`, the
short final batch, invalid sizes, invalid learning settings and
intermediate overflow.

## Limitations

Univariate regression only, fixed learning rate, unshuffled batches,
no momentum/Adam, gradients from half-MSE rather than classification
loss, no gradient clipping or convergence detection. Fixed data order
can yield biased updates for structured datasets. The use of mini-batches
**does not imply** a runtime speedup on small CPU workloads.

## References

Bottou, L. (2010). Large-scale machine learning with stochastic gradient
descent. In Y. Lechevallier & G. Saporta (Eds.), *Proceedings of COMPSTAT'2010*
(pp. 177–186). Physica-Verlag HD.
https://doi.org/10.1007/978-3-7908-2604-3_16
