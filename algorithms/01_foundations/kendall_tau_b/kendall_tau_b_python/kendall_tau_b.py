"""Kendall tau-b rank association with explicit tie accounting."""
import math


def kendall_tau_b(x, y):
    """Return tie-adjusted tau-b for aligned bounded finite observations."""
    if not isinstance(x, (list, tuple)) or not isinstance(y, (list, tuple)):
        raise ValueError("two paired sequences required")
    n = len(x)
    if not 2 <= n <= 128 or n != len(y):
        raise ValueError("equal lengths in 2..128 required")

    def checked(seq):
        out = []
        for value in seq:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("numeric observations required")
            try:
                v = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable observation") from exc
            if not math.isfinite(v) or abs(v) > 1_000_000:
                raise ValueError("observation outside finite bounds")
            out.append(v)
        return out

    a, b = checked(x), checked(y)
    concordant = discordant = tie_x = tie_y = 0
    for i in range(n):
        for j in range(i + 1, n):
            sx = (a[i] > a[j]) - (a[i] < a[j])
            sy = (b[i] > b[j]) - (b[i] < b[j])
            if sx == 0 and sy == 0:
                continue
            if sx == 0:
                tie_x += 1
            elif sy == 0:
                tie_y += 1
            elif sx == sy:
                concordant += 1
            else:
                discordant += 1
    denominator = math.sqrt(
        (concordant + discordant + tie_x)
        * (concordant + discordant + tie_y)
    )
    if denominator == 0:
        raise ValueError("tau-b undefined for constant rank feature")
    return max(-1.0, min(1.0, (concordant - discordant) / denominator))
