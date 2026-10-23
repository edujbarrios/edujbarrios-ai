"""Exact Euclidean k-nearest neighbor indices, with stable tie-breaking."""

import math
from typing import Sequence


def _vector(values: Sequence[float], size: int) -> list[float]:
    if not isinstance(values, (list, tuple)) or len(values) != size:
        raise ValueError("invalid vector dimension")
    result = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (float, int)):
            raise ValueError("vector components must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("components must be finite") from exc
        if not math.isfinite(number):
            raise ValueError("components must be finite")
        result.append(number)
    return result


def knn(samples: Sequence[Sequence[float]],
        query: Sequence[float], k: int) -> list[int]:
    """Return zero-based indices of nearest samples, sorted by (distance, index)."""
    if not isinstance(samples, (list, tuple)) or not samples:
        raise ValueError("samples must be nonempty")
    if not isinstance(query, (list, tuple)) or not query:
        raise ValueError("query must be nonempty")
    if isinstance(k, bool) or not isinstance(k, int) or not 1 <= k <= len(samples):
        raise ValueError("k must be an integer in 1..number of samples")
    target = _vector(query, len(query))
    ranked = []
    for index, sample in enumerate(samples):
        row = _vector(sample, len(target))
        distance = 0.0
        for a, b in zip(row, target):
            difference = a - b
            if not math.isfinite(difference):
                raise ValueError("distance overflow")
            distance = math.hypot(distance, difference)
            if not math.isfinite(distance):
                raise ValueError("distance overflow")
        ranked.append((distance, index))
    ranked.sort()
    return [index for _, index in ranked[:k]]
