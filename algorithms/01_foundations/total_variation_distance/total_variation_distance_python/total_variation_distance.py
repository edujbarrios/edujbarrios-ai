"""Total variation distance for normalized discrete distributions."""
import math
from typing import Sequence


def total_variation_distance(first: Sequence[float], second: Sequence[float]) -> float:
    """Return TV distance between finite probability vectors."""
    def validate(values: Sequence[float]) -> list[float]:
        if not isinstance(values, (list, tuple)) or not values:
            raise ValueError("nonempty probability vector required")
        result = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("finite real probability required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable probability") from exc
            if not math.isfinite(number) or not 0.0 <= number <= 1.0:
                raise ValueError("probabilities must be finite and in [0, 1]")
            result.append(number)
        if abs(math.fsum(result) - 1.0) > 1e-9:
            raise ValueError("probabilities must sum to one")
        return result

    p, q = validate(first), validate(second)
    if len(p) != len(q):
        raise ValueError("distributions must have equal length")
    distance = 0.5 * math.fsum(abs(x - y) for x, y in zip(p, q))
    return min(1.0, max(0.0, distance))
