"""Finite real dot product implemented without third-party dependencies."""

import math
from typing import Sequence


def dot_product(left: Sequence[float], right: Sequence[float]) -> float:
    """Return a finite dot product; raise ValueError on invalid input."""
    if len(left) != len(right):
        raise ValueError("vectors must have equal lengths")

    total = 0.0
    for a, b in zip(left, right):
        if any(isinstance(x, bool) or not isinstance(x, (int, float)) for x in (a, b)):
            raise ValueError("vector values must be real numbers")
        try:
            x, y = float(a), float(b)
        except OverflowError as exc:
            raise ValueError("values must be finite") from exc
        if not (math.isfinite(x) and math.isfinite(y)):
            raise ValueError("values must be finite")
        product = x * y
        total += product
        if not (math.isfinite(product) and math.isfinite(total)):
            raise ValueError("dot product overflow")
    return total
