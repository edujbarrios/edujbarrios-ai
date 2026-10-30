"""Coordinate-wise soft thresholding for finite real vectors."""
import math
from typing import Sequence

def soft_thresholding(values: Sequence[float], threshold: float) -> list[float]:
    """Apply L1 shrinkage; raise ValueError for invalid inputs."""
    if isinstance(threshold, bool) or not isinstance(threshold, (int, float)):
        raise ValueError("threshold must be finite and nonnegative")
    try:
        lam = float(threshold)
    except OverflowError as exc:
        raise ValueError("threshold is not representable") from exc
    if not math.isfinite(lam) or lam < 0:
        raise ValueError("threshold must be finite and nonnegative")
    if len(values) == 0:
        raise ValueError("values must be nonempty")
    result = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("values must be finite real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("value is not representable") from exc
        if not math.isfinite(x):
            raise ValueError("values must be finite")
        result.append(x)
    return [x - lam if x > lam else x + lam if x < -lam else 0.0 for x in result]
