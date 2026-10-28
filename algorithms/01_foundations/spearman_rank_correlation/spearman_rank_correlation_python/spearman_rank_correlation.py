"""Tie-aware Spearman correlation using explicit pairwise midranks."""
import math


def spearman_rank_correlation(x, y):
    """Return Spearman rho; reject constant ranks and invalid input."""
    if not isinstance(x, (list, tuple)) or not isinstance(y, (list, tuple)):
        raise ValueError("two numeric sequences required")
    n = len(x)
    if not 2 <= n <= 128 or len(y) != n:
        raise ValueError("equal lengths between 2 and 128 required")

    def checked(seq):
        result = []
        for value in seq:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("finite real observations required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable observation") from exc
            if not math.isfinite(number) or abs(number) > 1_000_000:
                raise ValueError("observation outside allowed range")
            result.append(number)
        return result

    def midranks(values):
        return [
            sum(other < current for other in values)
            + (sum(other == current for other in values) + 1) / 2
            for current in values
        ]

    rx, ry = midranks(checked(x)), midranks(checked(y))
    mean = (n + 1) / 2
    dx = [r - mean for r in rx]
    dy = [r - mean for r in ry]
    sxx = sum(t * t for t in dx)
    syy = sum(t * t for t in dy)
    if sxx == 0 or syy == 0:
        raise ValueError("correlation undefined for constant ranks")
    rho = (sum(a * b for a, b in zip(dx, dy)) / math.sqrt(sxx)) / math.sqrt(syy)
    if not math.isfinite(rho):
        raise ValueError("nonfinite result")
    return max(-1.0, min(1.0, rho))
