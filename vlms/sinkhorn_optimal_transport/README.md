# Sinkhorn Optimal Transport for Cross-Modal Alignment

## Why it belongs in AI

Aligning visual patches and text tokens can be formulated as a transport
problem: distribute matching mass across a patch–token cost matrix while
respecting both modalities' total mass. **Sinkhorn iteration** efficiently
approximates entropy-regularized optimal transport using alternating
scalings (Cuturi, 2013).

## Algorithm

Given a nonnegative cost matrix `C` of shape `m x n` and
regularization `epsilon > 0`, find a transport plan `P`
approximately minimizing

```text
sum_ij P[i,j] * C[i,j] + epsilon * sum_ij P[i,j] * (log(P[i,j]) - 1)
```

subject to `P`'s row sums being `1/m` and column sums `1/n`.
The stabilized algorithm uses log-kernel `logK = -C / epsilon`:

```text
log_u[i] = -log(m) - logsumexp_j(logK[i,j] + log_v[j])
log_v[j] = -log(n) - logsumexp_i(logK[i,j] + log_u[i])
P[i,j]   = exp(log_u[i] + logK[i,j] + log_v[j])
```

Alternate updates until the largest marginal deviation is within
`tolerance`, or `max_iterations` is reached. Return the
plan, its transport cost `sum_ij P[i,j] C[i,j]`, and convergence
metadata. `converged=False` indicates the returned plan has not yet
satisfied the requested marginal tolerance.

## Contract

- `sinkhorn_optimal_transport(costs, regularization=0.5,
  max_iterations=1000, tolerance=1e-8)` takes a rectangular matrix
  of `1..32` rows and `1..32` columns.
- Every cost must be a finite, non-boolean real in `[0, 20]`.
  Regularization must be in `[0.05, 10]`; `max_iterations`
  an integer in `1..2000`; `tolerance` finite in
  `[1e-12, 0.1]`.
- Output keys are `plan`, `transport_cost`, `iterations`,
  `marginal_error`, and `converged`. The plan is nonnegative
  and approximately satisfies uniform marginals when converged.
- Invalid input raises `ValueError`. No randomization, network
  access or input mutation. Computations use binary64.

## Worked example

With `costs=[[0,2],[2,0]]` and `regularization=0.5`,
the plan places most of each row's mass on the diagonal.
Both rows and columns sum to approximately 0.5.

For all-zero costs in a 2-by-2 matrix, the plan is uniform:
`[[0.25,0.25],[0.25,0.25]]`.

## Complexity

For `I` iterations and matrix size `m x n`, time is
**O(I mn)** and auxiliary space **O(mn + m + n)**.
This is an educational dense implementation, not a batched GPU solver.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [sinkhorn_optimal_transport_python](sinkhorn_optimal_transport_python/) |

Other languages are not yet implemented.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover uniform and diagonal-favoring plans, rectangular
marginals, early stopping with nonconvergence, invalid costs,
parameter bounds, and input immutability.

## Limitations

This example starts with a **precomputed cost matrix**, not image or text
embeddings. Uniform marginals may not capture padding or differing token
importance; the entropic solution is diffuse compared with unregularized
optimal transport. Low regularization can converge slowly, so inspect
`converged` and `marginal_error` rather than assuming exact feasibility.
No gradients or large-scale GPU acceleration are included.

## References

Cuturi, M. (2013). Sinkhorn distances: Lightspeed computation of optimal
transport. In *Advances in Neural Information Processing Systems*
(Vol. 26). https://proceedings.neurips.cc/paper/2013/hash/af21d0c97db2e27e13572cbf59eb343d-Abstract.html
