# Two-Component Gaussian Mixture with Expectation-Maximization

## Why it belongs in this repository

Gaussian mixture models perform **soft clustering**: each observation has a
probability of belonging to each cluster rather than a hard label. Their
expectation-maximization (EM) learning procedure illustrates latent-variable
optimization, a foundational technique in probabilistic AI (Dempster et
al., 1977).

## Algorithm

This teaching implementation fits **exactly two univariate Gaussian
components** with parameters `weight[j]`, `mean[j]`, and `variance[j]`.

For each observation `x[i]`, the E-step computes the responsibility

```text
r[i,j] = weight[j] * NormalPDF(x[i] | mean[j], variance[j])
         / sum_h weight[h] * NormalPDF(x[i] | mean[h], variance[h])
```

in the **log domain** using a two-term log-sum-exp operation.
The M-step updates:

```text
mass[j] = sum_i r[i,j]
weight[j] = mass[j] / n
mean[j] = sum_i r[i,j] * x[i] / mass[j]
variance[j] = max(sum_i r[i,j] * (x[i] - mean[j])^2 / mass[j], 1e-6)
```

Initialization is deterministic: the minimum and maximum observations are
the initial means, both weights start at `0.5`, and both variances start at
the population variance of the data, floored at `1e-6`.

Stop when the absolute mixture log-likelihood change is at most `tolerance`
or after `max_iterations` updates. If a component's effective mass falls
to `1e-12` or less, stop without a further update. Sort components by
increasing mean in the returned result to make labels deterministic.

## Contract

- Input: list or tuple of **2 to 4096** finite, non-boolean observations
  with absolute value at most `1_000_000` and at least two distinct values.
- `max_iterations`: integer in `1..1000`, default 100.
  `tolerance`: finite nonnegative real value, default `1e-8`.
- Output dictionary with ordered `means`, `weights`, `variances`,
  scalar `log_likelihood`, and actual `iterations` completed.
- Invalid inputs or nonfinite likelihood raise `ValueError`. Data are
  never modified. All arithmetic uses binary64 floating point.
- The minimum variance floor prevents singular density estimates and
  modifies the unrestricted Gaussian-mixture maximum-likelihood objective.

## Worked example

`[-5.2, -5.0, -4.8, 4.8, 5.0, 5.2]` produces two
components with means near `-5` and `5`, each with weight near `0.5`.
The model reports a finite mixture log-likelihood.

## Complexity

At most `I` EM iterations over `n` scalar samples.
**Time:** `O(n I)`; **auxiliary space:** `O(n)` for first-component
responsibilities (plus constant-size parameters).

## Implementations

| Language | Folder |
| --- | --- |
| Python | [gaussian_mixture_em_1d_python](gaussian_mixture_em_1d_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests exercise separated clusters, weights, variance floor, deterministic
output, iteration limit, invalid parameters, nonfinite values and input
immutability.

## Limitations

This is not a general mixture-model library: there are only two
one-dimensional components. The likelihood is nonconvex and EM may reach
a local optimum. Initialization and the variance floor affect results.
No automatic component-count selection, covariance matrices, priors,
mini-batches or production scalability are included.

## References

Dempster, A. P., Laird, N. M., & Rubin, D. B. (1977). Maximum likelihood
from incomplete data via the EM algorithm. *Journal of the Royal Statistical
Society: Series B (Methodological), 39*(1), 1–22.
https://doi.org/10.1111/j.2517-6161.1977.tb01600.x
