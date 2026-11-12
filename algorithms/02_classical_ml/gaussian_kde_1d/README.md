# One-dimensional Gaussian Kernel Density Estimation

## Why it belongs in this repository

Kernel density estimation (KDE) is a nonparametric method for estimating a probability distribution from observations. Gaussian KDE provides a smooth density estimate that can be used for exploratory data analysis, anomaly scoring and as a component of probabilistic machine learning (Silverman, 1986).

## Mathematical definition

For n real-valued observations x[i], query location q and positive bandwidth h:

    f_hat(q) = 1/(n*h*sqrt(2*pi)) * sum_i exp(-0.5*((q-x[i])/h)^2)

Each sample contributes an equally weighted Gaussian kernel. This function evaluates the density at **one query point**; the bandwidth is supplied rather than estimated.

## Contract

- Input: 1..512 finite observations in [-1000, 1000], one finite query in [-1000, 1000], and finite bandwidth in [0.001, 1000].
- Output: a nonnegative finite density, potentially zero due to floating-point exponential underflow for far-away queries. No mutations.
- C: `bool gaussian_kde_1d(const double *samples, size_t n, double query, double bandwidth, double *output)`. Invalid input leaves output untouched.
- Python: a list/tuple of real-valued numeric observations (not booleans), returns float, raises ValueError on invalid input.
- Rust: f64 slice, returns Option<f64> (None on invalid input).
- R: one-dimensional numeric vector (not logical/matrix); invalid input raises error.
- Bounds keep squared standardized distances and the final normalization finite using binary64.

## Complexity

O(n) time, O(1) additional memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [gaussian_kde_1d_c](gaussian_kde_1d_c/) |
| Python | [gaussian_kde_1d_python](gaussian_kde_1d_python/) |
| Rust | [gaussian_kde_1d_rust](gaussian_kde_1d_rust/) |
| R | [gaussian_kde_1d_r](gaussian_kde_1d_r/) |

## Examples and tests

- Samples [0], q=0, h=1: `1/sqrt(2*pi)` (~0.3989422804).
- Samples [-1,1], q=0, h=1: `exp(-1/2)/sqrt(2*pi)` (~0.2419707245).
- Samples [1,1], q=1, h=2: `1/(2*sqrt(2*pi))`.
- Samples [0], q=1000, h=0.001: zero after exponential underflow.

Tests cover singleton and repeated samples, symmetric samples, extreme supported bandwidths, 512 values, and invalid values and lengths.

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

One-dimensional, equally weighted, fixed-bandwidth KDE only. No bandwidth selection, adaptive kernels, boundary correction, log-density, multivariate support, gradients or calibrated anomaly thresholds. Small densities can underflow to zero.

## References

Silverman, B. W. (1986). *Density estimation for statistics and data analysis*. Chapman and Hall.
