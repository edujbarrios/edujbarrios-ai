"""Numerically scaled cosine similarity without external dependencies."""

import math
from typing import Sequence


def cosine_similarity(left: Sequence[float], right: Sequence[float]) -> float:
    """Compare two finite nonzero vectors; raise ValueError for invalid input."""
    if len(left) != len(right) or not left:
        raise ValueError("vectors must be nonempty and have equal lengths")

    a_values = []
    b_values = []
    for a, b in zip(left, right):
        if any(isinstance(v, bool) or not isinstance(v, (float, int)) for v in (a, b)):
            raise ValueError("vector components must be real numbers")
        try:
            x, y = float(a), float(b)
        except OverflowError as exc:
            raise ValueError("components must be finite") from exc
        if not (math.isfinite(x) and math.isfinite(y)):
            raise ValueError("components must be finite")
        a_values.append(x)
        b_values.append(y)

    a_max = max(abs(x) for x in a_values)
    b_max = max(abs(x) for x in b_values)
    if a_max == 0.0 or b_max == 0.0:
        raise ValueError("cosine similarity is undefined for zero vectors")

    dot = left_sq = right_sq = 0.0
    for a, b in zip(a_values, b_values):
        x, y = a / a_max, b / b_max
        dot += x * y
        left_sq += x * x
        right_sq += y * y
    result = dot / (math.sqrt(left_sq) * math.sqrt(right_sq))
    if not math.isfinite(result):
        raise ValueError("similarity is not finite")
    return max(-1.0, min(1.0, result))
