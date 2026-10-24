# Adam Optimizer — One Update Step

## Why it belongs in the AI hub

**Adam (Adaptive Moment Estimation)** combines exponential averages of
gradients and squared gradients to obtain per-parameter learning rates.
It is widely used for training neural networks and illustrates how
stateful optimizers differ from simple gradient descent (Kingma & Ba, 2015).
This implementation performs **one explicit update**, making its state
and bias correction inspectable, not silently managing a training loop.

## Mathematical definition

Given a parameter vector `theta`, its current gradient vector `g`,
prior moment vectors `m` and `v`, and **1-based step index** `t`:

```text
m_new[i] = beta1*m[i] + (1-beta1)*g[i]
v_new[i] = beta2*v[i] + (1-beta2)*g[i]^2
m_hat[i] = m_new[i] / (1 - beta1^t)
v_hat[i] = v_new[i] / (1 - beta2^t)
theta_new[i] = theta[i] - learning_rate*m_hat[i] / (sqrt(v_hat[i]) + epsilon)
```

Use fixed `beta1=0.9`, `beta2=0.999`, `epsilon=1e-8`.
The caller chooses positive finite `learning_rate`.
For step `t=1` initialize `m=v=0`; subsequent calls must pass the
new moments and increment `t`. **State is not reset between steps**.

Example for `theta=[2,-3]`, `g=[1,-2]`, `m=v=[0,0]`, `t=1`,
`learning_rate=0.1`:

```text
m_new = [0.1, -0.2]
v_new = [0.001, 0.004]
theta_new ≈ [1.900000001, -2.9000000005]
```

## Shared contract

- Parameter, gradient, first-moment and second-moment vectors have the
  same **nonzero length n**; all entries must be finite real numbers.
- Second moments must be **nonnegative**; negative values are invalid.
  The 1-based integer `step >= 1`; learning rate must be finite > 0.
- Return updated parameter, first-moment and second-moment vectors.
  The input arrays are unchanged. C uses separate caller-owned buffers
  (three outputs of `n` elements); outputs must not overlap each other
  or inputs and are unchanged if any computation fails. Rust returns
  `Result<(Vec<f64>, Vec<f64>, Vec<f64>), ...>`; Python returns a tuple
  of lists; R returns a named list with `parameters`, `m`, `v`.
- Reject nonfinite input, invalid shapes, zero/nonpositive step,
  negative second moments and nonfinite intermediate products, moments,
  bias correction, divisions or updated parameters.
- This implementation **does not** use weight decay, gradient clipping,
  parameter groups, AMSGrad or mixed precision. Numeric results are
  approximate and may differ in the last bits between languages.
- A very large step may cause beta powers to underflow to zero; that is
  mathematically harmless for positive integer steps. An extreme gradient
  whose square overflows is rejected.

## Complexity

O(n) time and O(n) output storage, with O(1) additional scratch per
parameter beyond the returned vectors. C commits all three outputs only
after successful calculation using temporary arrays.

## Implementations

| Language | Path |
| --- | --- |
| C | [adam_optimizer_c](adam_optimizer_c/) |
| Python | [adam_optimizer_python](adam_optimizer_python/) |
| Rust | [adam_optimizer_rust](adam_optimizer_rust/) |
| R | [adam_optimizer_r](adam_optimizer_r/) |

## Verification

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover first and second sequential steps, zero gradients,
multi-parameter state, invalid moments/dimensions, nonfinite values and
overflow. This is an educational optimizer primitive rather than a
full training framework.

## Limitations

The step API cannot validate whether the supplied moments actually
correspond to the claimed step index; state management belongs to the
caller. Default hyperparameters are fixed. A standalone CUDA version
requires hardware tests and realistic benchmarks before inclusion.

## References

Kingma, D. P., & Ba, J. (2015). Adam: A method for stochastic optimization.
In *3rd International Conference on Learning Representations (ICLR)*.
https://arxiv.org/abs/1412.6980
