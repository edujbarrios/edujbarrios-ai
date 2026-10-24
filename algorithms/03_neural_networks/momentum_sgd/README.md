# Stochastic Gradient Descent with Momentum — One Update

## Motivation

**Momentum SGD** smooths optimization updates by accumulating a
velocity from current and previous gradients. It can accelerate
descent along persistent directions and damp oscillations. Momentum
is a classical improvement over plain gradient descent and
complements this catalogue's RMSProp and Adam reference optimizers
(Polyak, 1964).

The caller supplies a gradient produced elsewhere; this implementation
performs **one update** and returns the new velocity for the next step.
It does not implement a full neural-network training loop.

## Definition

For current parameters `theta`, current gradient `g`, previous
velocity `v`, and learning rate `eta > 0`, with **fixed momentum
coefficient beta = 0.9**:

```text
v_next[i] = 0.9*v[i] + g[i]
theta_next[i] = theta[i] - eta*v_next[i]
```

This is the **gradient-accumulation convention**: velocity has the
units of the gradient, and the learning rate multiplies the accumulated
velocity at the parameter update. It differs from the alternative
convention `v_next = beta*v + eta*g`; velocity values cannot be
mixed across these conventions. This is **not Nesterov momentum**.

Example, with `eta=0.1`:

```text
theta = [2,-3], gradient = [1,-2], velocity=[0,0]
v_next = [1,-2]
theta_next = [1.9,-2.8]

second step: gradient=[0,0], previous velocity=[1,-2]
v_next = [0.9,-1.8]
theta_next = [1.81,-2.62]
```

## Shared contract

- All three input vectors have the same **nonzero length** and
  contain only finite real values. The learning rate must be finite
  and strictly positive.
- Return two vectors of that same length: updated parameters and
  updated velocity. Do not mutate the input arrays.
- Reject empty or mismatched arrays, invalid learning rate,
  NaN/infinity, and nonfinite velocity products, accumulation, update
  terms or resulting parameters.
- C uses caller-owned **non-overlapping**, correctly sized output
  buffers and preserves both outputs on error. Rust returns
  `Result<(Vec<f64>,Vec<f64>),...>`; Python returns a pair of lists;
  R returns `list(parameters=..., velocity=...)`.
- Velocity may be **negative**. It is not RMSProp's nonnegative
  squared-gradient state. Initialize it to zeros for the first step,
  then reuse its updated value on every subsequent optimizer step.
- R uses base double vectors. Python rejects boolean-as-numeric
  inputs; cross-language results can differ slightly by rounding.

## Complexity

**O(n)** time and **O(n)** output storage for n parameters.
C uses temporary buffers to commit both outputs atomically.

## Implementations

| Language | Directory |
| --- | --- |
| C | [momentum_sgd_c](momentum_sgd_c/) |
| Python | [momentum_sgd_python](momentum_sgd_python/) |
| Rust | [momentum_sgd_rust](momentum_sgd_rust/) |
| R | [momentum_sgd_r](momentum_sgd_r/) |

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover two sequential updates with state persistence, zero
gradients, negative velocity, invalid shapes, learning-rate bounds
and finite-number overflow.

## Limitations

No minibatch creation, gradient estimation, schedule, Nesterov
lookahead, weight decay, clipping, optimizer state checkpoint,
model integration, GPU kernel or benchmark. This primitive does not
guarantee convergence or optimal hyperparameters.

## References

Polyak, B. T. (1964). Some methods of speeding up the convergence of
iteration methods. *USSR Computational Mathematics and Mathematical
Physics, 4*(5), 1–17.
https://doi.org/10.1016/0041-5553(64)90137-5
