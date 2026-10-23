"""One-dimensional ordinary least squares from first principles."""

import math
from typing import Sequence


def _convert(values: Sequence[float]) -> list[float]:
    result = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("values must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("values must be representable") from exc
        if not math.isfinite(number):
            raise ValueError("values must be finite")
        result.append(number)
    return result


def linear_regression(x: Sequence[float], y: Sequence[float]) -> tuple[float, float]:
    """Return (slope, intercept) for a nonconstant one-dimensional feature."""
    if len(x) != len(y) or len(x) < 2:
        raise ValueError("at least two equally sized observations required")
    xs, ys = _convert(x), _convert(y)
    mean_x = mean_y = 0.0
    for index, (a, b) in enumerate(zip(xs, ys), 1):
        dx, dy = a - mean_x, b - mean_y
        if not (math.isfinite(dx) and math.isfinite(dy)):
            raise ValueError("mean overflow")
        mean_x += dx / index
        mean_y += dy / index
        if not (math.isfinite(mean_x) and math.isfinite(mean_y)):
            raise ValueError("mean overflow")
    xx = xy = 0.0
    for a, b in zip(xs, ys):
        dx, dy = a - mean_x, b - mean_y
        squared, product = dx * dx, dx * dy
        xx += squared
        xy += product
        if not all(math.isfinite(v) for v in (dx, dy, squared, product, xx, xy)):
            raise ValueError("variance/covariance overflow")
    if xx == 0.0:
        raise ValueError("x must not be constant")
    slope = xy / xx
    intercept = mean_y - slope * mean_x
    if not (math.isfinite(slope) and math.isfinite(intercept)):
        raise ValueError("regression overflow")
    return slope, intercept
