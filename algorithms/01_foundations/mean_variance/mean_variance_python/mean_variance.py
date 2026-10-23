"""Online mean and population variance (Welford, 1962)."""

import math
from typing import Sequence


def mean_variance(values: Sequence[float]) -> tuple[float, float]:
    """Return (mean, population_variance) for nonempty finite real values."""
    if not values:
        raise ValueError("observations must be nonempty")
    mean = 0.0
    m2 = 0.0
    for count, value in enumerate(values, start=1):
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("observations must be real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("observations must be finite") from exc
        if not math.isfinite(x):
            raise ValueError("observations must be finite")
        delta = x - mean
        if not math.isfinite(delta):
            raise ValueError("intermediate overflow")
        mean += delta / count
        delta2 = x - mean
        increment = delta * delta2
        if not all(math.isfinite(v) for v in (mean, delta2, increment)):
            raise ValueError("intermediate overflow")
        m2 += increment
        if not math.isfinite(m2):
            raise ValueError("variance overflow")
    variance = m2 / len(values)
    if not math.isfinite(variance) or variance < 0.0:
        raise ValueError("variance is not finite")
    return mean, variance
