"""Canberra distance with zero-safe, scale-normalized coordinate terms."""
import math
from typing import Sequence


def canberra_distance(first: Sequence[float], second: Sequence[float]) -> float:
    """Return a nonnegative Canberra distance for finite nonempty vectors."""
    def validate(values: Sequence[float]) -> list[float]:
        if not isinstance(values, (tuple, list)) or not values:
            raise ValueError("nonempty real vector required")
        converted = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("finite real coordinates required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable coordinate") from exc
            if not math.isfinite(number):
                raise ValueError("finite real coordinates required")
            converted.append(number)
        return converted

    x, y = validate(first), validate(second)
    if len(x) != len(y):
        raise ValueError("vector lengths must match")
    terms = []
    for a, b in zip(x, y):
        scale = max(abs(a), abs(b))
        if scale == 0:
            terms.append(0.0)
        else:
            a_normalized, b_normalized = a / scale, b / scale
            terms.append(abs(a_normalized - b_normalized) /
                         (abs(a_normalized) + abs(b_normalized)))
    distance = math.fsum(terms)
    if not math.isfinite(distance):
        raise ValueError("nonfinite distance")
    return distance
