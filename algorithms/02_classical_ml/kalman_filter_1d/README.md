# One-Dimensional Kalman Filter

## Why it belongs in AI

A Kalman filter performs recursive **state estimation** from noisy
observations. It underlies tracking, sensor fusion, robotics, and
probabilistic time-series models (Kalman, 1960). This reference
implementation uses the simplest scalar random-walk model so each
prediction and measurement update can be inspected.

## Algorithm

Assume a latent state evolving by `state[t] = state[t-1] + noise[t]`
with independent Gaussian process noise of variance `q`. An observed
value satisfies `measurement[t] = state[t] + error[t]` with independent
Gaussian measurement noise variance `r`.

Starting with an initial mean `mu` and variance `p`, for each step:

```text
prior_mean = mu
prior_variance = p + q
if measurement is missing:
    mu, p = prior_mean, prior_variance
    gain = 0
else:
    gain = prior_variance / (prior_variance + r)
    mu = prior_mean + gain * (measurement - prior_mean)
    p = (1 - gain) * prior_variance
```

Return the updated mean, variance, and gain after each time step.
The covariance update follows the scalar form and does not include
an explicit Joseph-form matrix update.

## Contract

- `kalman_filter_1d(observations, process_variance,
  measurement_variance, initial_mean=0, initial_variance=1)`
  accepts a list/tuple of `1..4096` observations; each is either
  `None` (missing) or a finite, non-boolean real with absolute value
  at most `1_000_000`.
- Process variance `q` is finite in `[0, 1_000_000]`.
  Measurement variance `r` is finite in `(0, 1_000_000]`.
  Initial variance is finite in `[0, 1_000_000]`, and initial mean
  is finite with absolute value at most `1_000_000`.
- Returns `{"means": [...], "variances": [...], "gains": [...]}`,
  all of the same length as the input; gains are zero for missing
  measurements. Inputs are not modified.
- Invalid input or nonfinite numerical results raise `ValueError`.
  All operations use ordinary binary64 floating point.

## Worked example

```python
kalman_filter_1d([1, 2], 0, 1, initial_mean=0, initial_variance=1)
# {
#   "means": [0.5, 1.0],
#   "variances": [0.5, 0.33333333333333337],
#   "gains": [0.5, 0.3333333333333333]
# }
```

The first measurement moves the estimate from 0 to 0.5.
The second measurement updates it to 1 while uncertainty shrinks.

## Complexity

**O(T)** time for `T` observations and **O(T)** output storage;
the numerical filter state itself uses O(1) space.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [kalman_filter_1d_python](kalman_filter_1d_python/) |

Other languages are not yet implemented.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover the known scalar updates, missing observations, zero
process noise, zero prior uncertainty, deterministic output,
input immutability, and invalid parameter values.

## Limitations

This assumes scalar linear-Gaussian dynamics with an identity state
transition and identity observation matrix. It does not learn the noise
variances or implement smoothing, outlier rejection, multivariate
covariance, control inputs, nonlinear EKF/UKF updates, or numerical
stabilization for extreme scales. Good parameter choices matter:
uncalibrated variances can produce overconfident or sluggish estimates.

## References

Kalman, R. E. (1960). A new approach to linear filtering and prediction
problems. *Journal of Basic Engineering, 82*(1), 35–45.
https://doi.org/10.1115/1.3662552
