"""Numerically stable unweighted mean log-cosh regression objective."""
import math


def log_cosh_loss(target, prediction):
    """Compute mean log(cosh(prediction - target)) for finite paired data."""
    if not isinstance(target, (list, tuple)) or not isinstance(prediction, (list, tuple)):
        raise ValueError("paired numeric sequences required")
    n = len(target)
    if not 1 <= n <= 10_000 or len(prediction) != n:
        raise ValueError("equal lengths in 1..10000 required")

    def checked(seq):
        numbers = []
        for value in seq:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("finite numeric values required")
            try:
                x = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable numerical value") from exc
            if not math.isfinite(x) or abs(x) > 1_000_000:
                raise ValueError("value outside allowed range")
            numbers.append(x)
        return numbers

    a, b = checked(target), checked(prediction)
    total = 0.0
    for t, p in zip(a, b):
        magnitude = abs(p - t)
        total += max(
            0.0,
            magnitude + math.log1p(math.exp(-2 * magnitude)) - math.log(2),
        )
    return total / n
