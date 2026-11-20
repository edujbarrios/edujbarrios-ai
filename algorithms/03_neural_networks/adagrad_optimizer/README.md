# AdaGrad Optimizer Step

## Why it belongs in AI

AdaGrad adapts step sizes to each coordinate based on the historical magnitude
of its gradients. Coordinates that have accumulated large squared gradients
receive smaller future updates, useful for sparse features and online learning
(Duchi et al., 2011).

## Algorithm

For each coordinate i, given parameters w_i, current gradient g_i, and a
nonnegative accumulator h_i representing *previous* squared gradients:

    h_new[i] = h[i] + g[i]^2
    w_new[i] = w[i] - learning_rate * g[i] / (sqrt(h_new[i]) + epsilon)

This is one **stateful optimization step**. The caller supplies the next
step with the returned parameters and accumulator. No gradient computation,
batch management or random initialization is performed.

## Contract

- Inputs are equally long vectors of finite double-precision parameters,
  gradients and nonnegative accumulators. Empty vectors are accepted.
- Learning rate and epsilon must be finite **positive** numbers. Epsilon
  is placed **outside** the square root.
- Return updated parameters and accumulators, each with the original length.
- Reject mismatched lengths, nonfinite inputs, negative accumulators,
  nonfinite squared-gradient sums, nonfinite steps or nonfinite updated
  parameters. There is no silent clipping.
- The C API accepts separate nonoverlapping caller-owned output arrays,
  writes only after a successful full validation pass and leaves them
  unchanged on failure. Input and output buffers must not alias.
- Purely deterministic; no seed needed. All implementations use doubles.

Example: w=[1,-1], g=[2,-4], h=[0,0], learning_rate=0.1,
epsilon=1 gives h_new=[4,16] and w_new=[0.9333333333,-0.92].

## Complexity

O(n) time, O(n) returned state. The C implementation needs O(1)
auxiliary memory in addition to its two caller-owned output arrays.

## Implementations

| Language | Folder |
| --- | --- |
| C | [adagrad_optimizer_c](adagrad_optimizer_c/) |
| Python | [adagrad_optimizer_python](adagrad_optimizer_python/) |
| Rust | [adagrad_optimizer_rust](adagrad_optimizer_rust/) |
| R | [adagrad_optimizer_r](adagrad_optimizer_r/) |

Check metadata and run all declared language tests:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This educational implementation contains no momentum, weight decay,
bias correction, automatic differentiation or optimizer state management.
Naively squaring huge gradients can overflow, in which case the step is
rejected. Floating-point rounding can differ slightly across toolchains.

## References

Duchi, J., Hazan, E., & Singer, Y. (2011). Adaptive subgradient methods
for online learning and stochastic optimization. *Journal of Machine
Learning Research, 12*, 2121–2159. https://jmlr.org/papers/v12/duchi11a.html
