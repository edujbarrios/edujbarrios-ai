# AdamW Optimizer — One Update Step

## Motivation

**AdamW** is a variant of Adam that applies **decoupled weight decay**
rather than adding an L2 penalty to the gradient. Decoupling prevents
adaptive moment scaling from unintentionally altering the strength of
parameter shrinkage (Loshchilov & Hutter, 2019). This is a useful
reference alongside the catalogue's SGD, Momentum, RMSProp and Adam
optimizers.

This educational primitive makes **one explicit stateful update**,
not a complete training loop. The caller provides its gradient and
previous moment vectors, then retains the returned state.

## Mathematics

Let parameters `theta[i]`, gradient `g[i]`, previous first/second
moments `m[i]` and `v[i]`, strictly positive learning rate
`eta` and one-based integer step `t>=1`. Use fixed
`beta1=0.9`, `beta2=0.999`, `epsilon=1e-8` and
`weight_decay=0.01`:

```text
m_next[i] = beta1*m[i] + (1-beta1)*g[i]
v_next[i] = beta2*v[i] + (1-beta2)*g[i]^2

m_hat[i] = m_next[i] / (1-beta1^t)
v_hat[i] = v_next[i] / (1-beta2^t)

theta_next[i] = theta[i] - eta * (
    m_hat[i] / (sqrt(v_hat[i]) + epsilon)
    + weight_decay * theta[i]
)
```

In particular, **weight decay does not enter either moment**.
The decay uses the *old* parameter value, not the newly updated one.

Example for `theta=[2,-3]`, `g=[1,-2]`, `m=v=[0,0]`,
`eta=0.1`, `t=1`:

```text
m_next = [0.1,-0.2]
v_next = [0.001,0.004]
theta_next ≈ [1.898000001, -2.8970000005]
```

## Shared contract

- All four input vectors have equal **nonzero** length, and all values
  must be finite reals; every previous second moment must be >=0.
- `eta` is a finite positive scalar and `t` is a **one-based
  positive integer**, at most 1,000,000. Initialization for the first
  step uses zero moment vectors.
- Return three vectors (updated parameters, first moments, second
  moments), leaving every input unchanged.
- Reject malformed shapes, negative second moments, NaN/Inf, bad
  hyperparameters, nonfinite gradient squares, moment updates,
  bias-correction denominators, shrinkage, or final parameter values.
- C uses flat input and three **distinct caller-owned output arrays**
  with capacity n, all nonoverlapping with input; no output is altered
  on failure. Rust returns `Result<(Vec<f64>,Vec<f64>,Vec<f64>),...>`,
  Python returns a tuple of lists, and R a named list.
- Fixed weight decay 0.01 is intentionally visible in all code.
  This is not Adam with an L2-regularization term added to `g`.
- The API cannot prove the supplied moments correspond to the
  supplied t. State consistency is the caller's responsibility.

## Complexity

O(n) time and O(n) storage for updated vectors. C additionally keeps
temporary output buffers so errors never yield partially written state.

## Implementations

| Language | Directory |
| --- | --- |
| C | [adamw_optimizer_c](adamw_optimizer_c/) |
| Python | [adamw_optimizer_python](adamw_optimizer_python/) |
| Rust | [adamw_optimizer_rust](adamw_optimizer_rust/) |
| R | [adamw_optimizer_r](adamw_optimizer_r/) |

Run verification from the project root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover first and second stateful updates, zero gradients
(decay-only step), malformed vector dimensions, illegal moments
and numerical overflow.

## Limitations

No AMSGrad, parameter groups, learning-rate schedule, clipping,
mixed-precision training, complete backpropagation, fused kernel or
device-tested GPU implementation. Parameter decay is applied to all
parameters, including biases, because the primitive has no metadata
for excluding particular parameter groups.

## References

Loshchilov, I., & Hutter, F. (2019). Decoupled weight decay
regularization. In *7th International Conference on Learning
Representations*. https://arxiv.org/abs/1711.05101
