"""Stable L2 normalization for ordinary real-number sequences."""

import math
from typing import Sequence


def vector_normalization(values: Sequence[float]) -> list[float]:
    """Return a unit vector; raise ValueError for invalid or zero vectors."""
    if len(values) == 0:
        raise ValueError("cannot normalize an empty vector")
    numbers = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("components must be real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("components must be finite") from exc
        if not math.isfinite(number):
            raise ValueError("components must be finite")
        numbers.append(number)
    scale = max(abs(number) for number in numbers)
    if scale == 0.0:
        raise ValueError("cannot normalize a zero vector")
    squared = sum((number / scale) ** 2 for number in numbers)
    norm = math.sqrt(squared)
    return [(number / scale) / norm for number in numbers]
