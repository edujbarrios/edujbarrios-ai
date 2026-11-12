"""Univariate Gaussian kernel density estimation at one query point."""
import math


def gaussian_kde_1d(samples: list[float], query: float, bandwidth: float) -> float:
    """Return the equally weighted Gaussian-kernel density at query."""
    def finite_bounded(x: float, limit: float) -> bool:
        return (not isinstance(x, bool) and isinstance(x, (int, float)) and
                math.isfinite(x) and abs(x) <= limit)

    if not isinstance(samples, (list, tuple)) or not 1 <= len(samples) <= 512:
        raise ValueError("one to 512 numeric observations required")
    if not all(finite_bounded(x, 1000) for x in samples):
        raise ValueError("observations must be finite and in [-1000,1000]")
    if not finite_bounded(query, 1000) or not finite_bounded(bandwidth, 1000) \
            or bandwidth < 0.001:
        raise ValueError("bounded query and bandwidth in [0.001,1000] required")

    total = sum(math.exp(-0.5 * ((query - x) / bandwidth) ** 2)
                for x in samples)
    return total / (len(samples) * bandwidth * math.sqrt(2.0 * math.pi))
