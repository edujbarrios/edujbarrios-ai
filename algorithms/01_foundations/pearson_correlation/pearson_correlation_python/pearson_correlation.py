"""Two-pass Pearson product-moment correlation for bounded paired data."""
import math


def pearson_correlation(x, y):
    """Return Pearson r in [-1, 1]; reject invalid or constant features."""
    if not isinstance(x, (list, tuple)) or not isinstance(y, (list, tuple)):
        raise ValueError("two numeric sequences required")
    n = len(x)
    if n < 2 or n > 10_000 or len(y) != n:
        raise ValueError("equal lengths between 2 and 10000 required")

    def checked(values):
        result = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("finite numeric values required")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable value") from exc
            if not math.isfinite(number) or abs(number) > 1_000_000:
                raise ValueError("value outside allowed range")
            result.append(number)
        return result

    a, b = checked(x), checked(y)
    mx, my = sum(a) / n, sum(b) / n
    sxx = syy = sxy = 0.0
    for xi, yi in zip(a, b):
        dx, dy = xi - mx, yi - my
        sxx += dx * dx
        syy += dy * dy
        sxy += dx * dy
    if sxx <= 0.0 or syy <= 0.0:
        raise ValueError("correlation undefined for constant inputs")
    r = (sxy / math.sqrt(sxx)) / math.sqrt(syy)
    if not math.isfinite(r):
        raise ValueError("nonfinite result")
    return max(-1.0, min(1.0, r))
