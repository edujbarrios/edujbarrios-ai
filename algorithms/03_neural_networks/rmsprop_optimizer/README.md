# RMSProp Optimizer — One Update Step

## Why it belongs in AI

**RMSProp** adapts each parameter's learning rate according to an
exponential moving average of its squared gradients. Parameters with
repeatedly large gradients receive smaller effective steps, reducing
oscillation along steep directions. It is a foundational stateful
neural-network optimizer and contrasts with both plain gradient descent
and the repository's Adam implementation (Hinton et al., 2012).

This reference performs **one explicit update**, so callers can examine
and carry optimizer state from one step to the next.

## Mathematical definition

Given parameter vector `theta`, gradient `g`, old running squared
gradient `v`, positive learning rate `eta`, and fixed
`rho=0.9`, `epsilon=1e-8`:

```text
v_next[i] = rho*v[i] + (1-rho)*g[i]^2
theta_next[i] = theta[i] - eta*g[i]/(sqrt(v_next[i]) + epsilon)
```

Unlike Adam, RMSProp does not track a first-moment accumulator or use
bias correction. The caller provides previous `v`; initialize all
entries to zero for the first step. A second step **uses v_next**,
not the original zero state.

Example:

```text
theta = [2,-3]
g = [1,-2]
v = [0,0]
eta = 0.1

v_next = [0.1,0.4]
theta_next ≈ [1.68377224398,-2.68377223898]
```

The `epsilon` value prevents division by zero for zero gradients.

## Shared contract

- All three input vectors have the **same nonzero length** and finite
  real values. The previous squared-gradient state must be **>=0**
  elementwise. The learning rate is a finite real **strictly >0**.
- Return updated parameters and the new squared-gradient vector.
  Inputs are not modified.
- Reject empty/mismatched vectors, negative or nonfinite state, invalid
  learning rate, and nonfinite gradient squares, weighted moments,
  denominators, update steps or resulting parameters. Extremely large
  finite gradients can overflow when squared and are rejected.
- C requires caller-owned disjoint `theta_next[n]` and `v_next[n]`
  buffers, each with capacity `n`; both remain untouched on failure.
  Rust returns `Result<(Vec<f64>,Vec<f64>),...>`; Python returns
  `(parameters, squared_average)`; R returns a named list with
  these same two elements.
- This algorithm uses fixed decay/epsilon to keep tests identical
  across languages; it does **not** apply momentum in parameter
  space, decoupled weight decay, bias correction or gradient clipping.

## Complexity

O(n) time and O(n) output storage for n parameters. The C implementation
uses scratch arrays to guarantee atomic outputs. No ML framework,
randomness, CUDA runtime or model training loop is needed.

## Implementations

| Language | Directory |
| --- | --- |
| C | [rmsprop_optimizer_c](rmsprop_optimizer_c/) |
| Python | [rmsprop_optimizer_python](rmsprop_optimizer_python/) |
| Rust | [rmsprop_optimizer_rust](rmsprop_optimizer_rust/) |
| R | [rmsprop_optimizer_r](rmsprop_optimizer_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the first step, a second step with carried state, zero
gradients, input validation, negative state and overflowing finite
gradients. All floating comparisons use tolerances.

## Limitations

A single update cannot prove convergence, and the caller must preserve
the correct running state. No batch gradient calculation, trainable
network, learning-rate schedule, distributed optimizer or fused tensor
kernel is provided. GPU support requires device-tested implementation.

## References

Hinton, G. E., Srivastava, N., & Swersky, K. (2012). *Neural networks
for machine learning* [Lecture 6e: RMSProp]. Coursera.
https://www.cs.toronto.edu/~tijmen/csc321/slides/lecture_slides_lec6.pdf
