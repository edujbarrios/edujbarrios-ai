"""Manhattan (L1) distance of finite real vectors."""

import math
from typing import Sequence


def manhattan_distance(left: Sequence[float], right: Sequence[float]) -> float:
    """Return sum of absolute differences or raise ValueError."""
    if len(left) != len(right):
        raise ValueError("vectors must have equal lengths")
    total = 0.0
    for a, b in zip(left, right):
        if any(isinstance(x, bool) or not isinstance(x, (int, float)) for x in (a, b)):
            raise ValueError("coordinates must be real, not boolean")
        try:
            x, y = float(a), float(b)
        except OverflowError as exc:
            raise ValueError("coordinates must be finite") from exc
        if not (math.isfinite(x) and math.isfinite(y)):
            raise ValueError("coordinates must be finite")
        delta = x - y
        if not math.isfinite(delta):
            raise ValueError("distance overflow")
        total += abs(delta)
        if not math.isfinite(total):
            raise ValueError("distance overflow")
    return total
