"""Numerically stable mean binary cross-entropy directly from logits."""
import math


def binary_cross_entropy_with_logits(logits, targets):
    """Compute stable mean BCE for finite logits and targets in [0, 1]."""
    if not isinstance(logits, (list, tuple)) or not isinstance(targets, (list, tuple)):
        raise ValueError("two numeric sequences required")
    n = len(logits)
    if not 1 <= n <= 10_000 or len(targets) != n:
        raise ValueError("equal lengths between 1 and 10000 required")

    def checked(sequence, target=False):
        result = []
        for x in sequence:
            if isinstance(x, bool) or not isinstance(x, (int, float)):
                raise ValueError("finite real values required")
            try:
                v = float(x)
            except OverflowError as exc:
                raise ValueError("unrepresentable value") from exc
            if not math.isfinite(v) or (not 0 <= v <= 1 if target else abs(v) > 1_000_000):
                raise ValueError("value outside declared bounds")
            result.append(v)
        return result

    zs, ys = checked(logits), checked(targets, target=True)
    total = 0.0
    for z, y in zip(zs, ys):
        term = max(z, 0.0) - z * y + math.log1p(math.exp(-abs(z)))
        total += max(0.0, term)
    return total / n
