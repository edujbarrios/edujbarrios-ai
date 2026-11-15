"""Scalar random-walk Kalman filter with optional missing observations."""

import math


def _bounded_number(value, name, minimum, maximum, open_minimum=False):
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"{name} must be a bounded finite number")
    try:
        number = float(value)
    except (OverflowError, ValueError):
        raise ValueError(f"{name} must be a bounded finite number") from None
    if (not math.isfinite(number) or number > maximum
            or (number <= minimum if open_minimum else number < minimum)):
        raise ValueError(f"{name} must be a bounded finite number")
    return number


def kalman_filter_1d(observations, process_variance, measurement_variance,
                     initial_mean=0.0, initial_variance=1.0):
    """Return posterior means, variances, and gains after each time step."""
    if not isinstance(observations, (list, tuple)) or not 1 <= len(observations) <= 4096:
        raise ValueError("observations must contain 1..4096 elements")

    q = _bounded_number(process_variance, "process_variance", 0, 1_000_000)
    r = _bounded_number(measurement_variance, "measurement_variance",
                        0, 1_000_000, open_minimum=True)
    mean = _bounded_number(initial_mean, "initial_mean", -1_000_000, 1_000_000)
    variance = _bounded_number(initial_variance, "initial_variance", 0, 1_000_000)
    values = [
        None if value is None else
        _bounded_number(value, "measurement", -1_000_000, 1_000_000)
        for value in observations
    ]
    means, variances, gains = [], [], []
    for observed in values:
        predicted_variance = variance + q
        if observed is None:
            variance = predicted_variance
            gain = 0.0
        else:
            gain = predicted_variance / (predicted_variance + r)
            mean += gain * (observed - mean)
            variance = (1.0 - gain) * predicted_variance
        if not (math.isfinite(mean) and math.isfinite(variance)
                and math.isfinite(gain) and variance >= 0):
            raise ValueError("nonfinite or negative estimated covariance")
        means.append(mean)
        variances.append(variance)
        gains.append(gain)
    return {"means": means, "variances": variances, "gains": gains}
