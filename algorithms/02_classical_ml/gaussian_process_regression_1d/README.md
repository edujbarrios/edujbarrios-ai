# One-Dimensional Gaussian Process Regression

## Why it belongs in AI

Gaussian process regression is a probabilistic nonlinear model that provides
both predictions and uncertainty estimates. It is useful for Bayesian
optimization, active learning, and small-data regression (Rasmussen &
Williams, 2006).

## Algorithm

Assume a zero-mean Gaussian process with the RBF covariance

```text
k(x, z) = signal_variance * exp(-0.5 * ((x - z) / length_scale)^2)
```

For training samples `x[0..n-1]` and targets `y`, form
`K[i,j] = k(x[i],x[j])`. Add `noise_variance + jitter`
to the diagonal and factorize `K + diagonal = L L^T`
with a Cholesky decomposition. Solve `L L^T alpha = y`.

For each test input `x*` define `k* = [k(x*,x[i])]`.
Compute the posterior latent mean `mu* = k*^T alpha` and
variance `v* = k(x*,x*) - ||L^(-1) k*||^2`.
Tiny negative variances caused by numerical roundoff are clamped to zero.

## Contract

- `gaussian_process_regression_1d(x_train, y_train, x_test,
  length_scale=1, signal_variance=1, noise_variance=1e-2)`
  accepts lists or tuples of `1..64` aligned training inputs and targets,
  and `0..1024` test points.
- Every input and target must be a finite non-boolean real with
  absolute value at most 1000; duplicate training inputs are allowed.
- `length_scale` must be finite in `[1e-4, 1e4]`;
  `signal_variance` finite and positive in `(0, 1e6]`;
  `noise_variance` finite in `[0, 1e6]`.
- To prevent a singular covariance matrix, the training diagonal includes
  `jitter = 1e-9 * max(1, signal_variance)` in addition to noise.
- Returns `{"means": [...], "variances": [...]}` for the *latent
  function* (not noisy observations), in test-point order.
- Invalid inputs or nonfinite numerical results raise `ValueError`.
  Inputs are never mutated. Numerical calculations use binary64.

## Worked example

At a single training observation `(x=0, y=2)`, RBF variance 1 and
noise variance 0.25, the posterior at `x=0` is approximately
`mean=1.6`, `variance=0.2` (with a negligible jitter adjustment).
Far outside the kernel's length scale, the posterior returns toward
the zero prior mean and unit prior variance.

## Complexity

Building/factorizing the covariance takes **O(n³)** time and
O(n²) memory. Predictions at `m` points take **O(m n²)**
time for triangular solves and **O(n² + m)** total auxiliary memory
including output vectors.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [gaussian_process_regression_1d_python](gaussian_process_regression_1d_python/) |

Other languages are not yet implemented.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover the analytic single-sample posterior, prior recovery
far from the data, duplicate inputs, different noise levels,
empty query vectors, invalid parameters and input immutability.

## Limitations

This is an **exact small-data regression** example, not a scalable GP.
It fixes the prior mean to zero and does not learn hyperparameters,
handle derivatives, use inducing points, or expose posterior covariance
between test points. Bounds and jitter are educational numerical guards.
Uncertainty is conditional on the chosen kernel and noise assumptions;
distribution shift and misspecification can make it misleading.

## References

Rasmussen, C. E., & Williams, C. K. I. (2006). *Gaussian processes
for machine learning*. MIT Press. https://gaussianprocess.org/gpml/
