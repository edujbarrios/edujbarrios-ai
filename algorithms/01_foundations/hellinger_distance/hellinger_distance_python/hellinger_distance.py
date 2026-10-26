"""Hellinger distance for discrete probability distributions."""
import math
from typing import Sequence

_TOLERANCE = 1e-9


def hellinger_distance(first: Sequence[float], second: Sequence[float]) -> float:
    """Return symmetric distance in [0, 1] between probability vectors."""
    def checked(data: Sequence[float]) -> list[float]:
        if not isinstance(data, (list, tuple)) or not data:
            raise ValueError("nonempty probability vector required")
        values = []
        for item in data:
            if isinstance(item, bool) or not isinstance(item, (int, float)):
                raise ValueError("finite nonnegative probabilities required")
            try:
                number = float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable probability") from exc
            if not math.isfinite(number) or not 0.0 <= number <= 1.0:
                raise ValueError("probabilities must be finite and in [0,1]")
            values.append(number)
        if abs(math.fsum(values) - 1.0) > _TOLERANCE:
            raise ValueError("probabilities must sum to one")
        return values

    p, q = checked(first), checked(second)
    if len(p) != len(q):
        raise ValueError("probability vector lengths differ")
    squared = math.fsum((math.sqrt(a) - math.sqrt(b)) ** 2
                        for a, b in zip(p, q))
    return min(1.0, max(0.0, math.sqrt(squared / 2.0)))
