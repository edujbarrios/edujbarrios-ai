"""Chebyshev (L-infinity) distance for finite vectors."""

import math
from typing import Sequence


def chebyshev_distance(left: Sequence[float], right: Sequence[float]) -> float:
    """Return the maximum coordinate difference (zero for empty vectors)."""
    if len(left) != len(right):
        raise ValueError("vectors must have equal lengths")
    maximum = 0.0
    for a, b in zip(left, right):
        if any(isinstance(x, bool) or not isinstance(x, (int, float)) for x in (a, b)):
            raise ValueError("coordinates must be real numbers")
        try:
            x, y = float(a), float(b)
        except OverflowError as exc:
            raise ValueError("coordinates must be finite") from exc
        if not (math.isfinite(x) and math.isfinite(y)):
            raise ValueError("coordinates must be finite")
        difference = x - y
        if not math.isfinite(difference):
            raise ValueError("distance overflow")
        maximum = max(maximum, abs(difference))
    return maximum
