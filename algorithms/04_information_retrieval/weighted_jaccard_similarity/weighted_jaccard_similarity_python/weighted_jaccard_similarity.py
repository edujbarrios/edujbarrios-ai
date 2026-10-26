"""Weighted Jaccard similarity for aligned nonnegative feature vectors."""
import math
from typing import Sequence


def weighted_jaccard_similarity(first: Sequence[float], second: Sequence[float]) -> float:
    """Return min/max weighted overlap in [0, 1], with zero/zero = 1."""
    def check(values: Sequence[float]) -> list[float]:
        if not isinstance(values, (list, tuple)) or not values:
            raise ValueError("nonempty vector required")
        result = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("nonnegative real weights required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable weight") from exc
            if not math.isfinite(number) or number < 0.0:
                raise ValueError("finite nonnegative weights required")
            result.append(number)
        return result

    x, y = check(first), check(second)
    if len(x) != len(y):
        raise ValueError("feature dimensions must match")
    scale = max(max(x), max(y))
    if scale == 0.0:
        return 1.0
    numerator = math.fsum(min(a, b) / scale for a, b in zip(x, y))
    denominator = math.fsum(max(a, b) / scale for a, b in zip(x, y))
    similarity = numerator / denominator
    if not math.isfinite(similarity):
        raise ValueError("nonfinite similarity")
    return min(1.0, max(0.0, similarity))
