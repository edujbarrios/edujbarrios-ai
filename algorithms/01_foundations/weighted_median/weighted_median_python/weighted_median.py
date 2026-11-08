"""Lower weighted median for finite numeric observations."""
import math


def weighted_median(
    values: list[float] | tuple[float, ...],
    weights: list[float] | tuple[float, ...],
) -> float:
    """Return first sorted value whose cumulative weight is at least half."""
    if (not isinstance(values, (list, tuple))
            or not isinstance(weights, (list, tuple))
            or not 1 <= len(values) <= 512 or len(values) != len(weights)):
        raise ValueError("expected equal-length numeric sequences of size 1..512")

    pairs = []
    for value, weight in zip(values, weights):
        if (isinstance(value, bool) or isinstance(weight, bool)
                or not isinstance(value, (int, float))
                or not isinstance(weight, (int, float))):
            raise ValueError("values and weights must be finite real numbers")
        try:
            x, w = float(value), float(weight)
        except OverflowError as exc:
            raise ValueError("value is outside finite floating-point range") from exc
        if not math.isfinite(x) or not math.isfinite(w) or w < 0.0:
            raise ValueError("values must be finite and weights nonnegative")
        pairs.append((x, w))

    pairs.sort(key=lambda p: p[0])
    total = 0.0
    for _, weight in pairs:
        total += weight
        if not math.isfinite(total):
            raise ValueError("weight sum overflow")
    if total <= 0.0:
        raise ValueError("total weight must be positive")
    cumulative = 0.0
    for value, weight in pairs:
        cumulative += weight
        if cumulative >= total - cumulative:
            return value
    raise ValueError("invalid weight sum")
