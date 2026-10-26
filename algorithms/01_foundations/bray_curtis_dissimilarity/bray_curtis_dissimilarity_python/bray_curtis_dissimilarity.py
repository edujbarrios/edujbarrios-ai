"""Bray–Curtis dissimilarity for finite nonnegative feature vectors."""
import math
from typing import Sequence


def bray_curtis_dissimilarity(first: Sequence[float], second: Sequence[float]) -> float:
    """Return value in [0,1]; two all-zero vectors have dissimilarity 0."""
    def validate(values: Sequence[float]) -> list[float]:
        if not isinstance(values, (list, tuple)) or not values:
            raise ValueError("nonempty feature vector required")
        result = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("nonnegative finite real weights required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable weight") from exc
            if not math.isfinite(number) or number < 0.0:
                raise ValueError("nonnegative finite real weights required")
            result.append(number)
        return result

    a, b = validate(first), validate(second)
    if len(a) != len(b):
        raise ValueError("feature lengths must match")
    scale = max(max(a), max(b))
    if scale == 0.0:
        return 0.0
    numerator = math.fsum(abs(x / scale - y / scale) for x, y in zip(a, b))
    denominator = math.fsum(x / scale + y / scale for x, y in zip(a, b))
    value = numerator / denominator
    if not math.isfinite(value):
        raise ValueError("nonfinite result")
    return min(1.0, max(0.0, value))
