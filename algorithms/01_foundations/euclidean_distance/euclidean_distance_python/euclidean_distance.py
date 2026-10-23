"""Stable Euclidean distance between finite real-valued sequences."""

import math
from typing import Sequence


def euclidean_distance(left: Sequence[float], right: Sequence[float]) -> float:
    """Return an L2 distance; reject invalid dimensions or non-finite values."""
    if len(left) != len(right):
        raise ValueError("vectors must have equal lengths")

    distance = 0.0
    for a, b in zip(left, right):
        if any(isinstance(x, bool) or not isinstance(x, (int, float)) for x in (a, b)):
            raise ValueError("vector values must be real numbers")
        try:
            x, y = float(a), float(b)
        except OverflowError as exc:
            raise ValueError("values must be finite") from exc
        if not (math.isfinite(x) and math.isfinite(y)):
            raise ValueError("values must be finite")
        difference = x - y
        if not math.isfinite(difference):
            raise ValueError("distance overflow")
        distance = math.hypot(distance, difference)
        if not math.isfinite(distance):
            raise ValueError("distance overflow")
    return distance
