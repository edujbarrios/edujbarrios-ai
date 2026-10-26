"""Exponential moving average of a finite numeric sequence."""

import math
from typing import Sequence


def exponential_moving_average(values: Sequence[float], alpha: float) -> list[float]:
    """Return a smoothed series, starting at the first observation."""
    if not isinstance(values, (list, tuple)) or not values:
        raise ValueError("a nonempty numeric list or tuple is required")
    if isinstance(alpha, bool) or not isinstance(alpha, (int, float)):
        raise ValueError("alpha must be a finite real number in (0,1]")
    try:
        a = float(alpha)
    except OverflowError as exc:
        raise ValueError("alpha must be finite") from exc
    if not math.isfinite(a) or not 0 < a <= 1:
        raise ValueError("alpha must be in (0,1]")
    result: list[float] = []
    for item in values:
        if isinstance(item, bool) or not isinstance(item, (int, float)):
            raise ValueError("observations must be real numbers")
        try:
            x = float(item)
        except OverflowError as exc:
            raise ValueError("observations must be finite") from exc
        if not math.isfinite(x):
            raise ValueError("observations must be finite")
        value = x if not result else (1.0 - a) * result[-1] + a * x
        if not math.isfinite(value):
            raise ValueError("smoothed value is not finite")
        result.append(value)
    return result
