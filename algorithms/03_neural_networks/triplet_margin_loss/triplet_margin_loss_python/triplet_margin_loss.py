"""Euclidean margin ranking loss for anchor-positive-negative triplets."""
import math


def triplet_margin_loss(anchor: list[float], positive: list[float],
                        negative: list[float], margin: float) -> float:
    """Return max(0, norm(anchor-positive) - norm(anchor-negative) + margin)."""
    if isinstance(margin, bool) or not isinstance(margin, (int, float)) or \
            not math.isfinite(margin) or not 0 <= margin <= 1000:
        raise ValueError("finite margin from 0 to 1000 required")
    vectors = (anchor, positive, negative)
    if not isinstance(anchor, (list, tuple)) or not 1 <= len(anchor) <= 512:
        raise ValueError("1..512 dimensional embedding required")
    for values in vectors:
        if not isinstance(values, (list, tuple)) or len(values) != len(anchor):
            raise ValueError("equally sized vectors required")
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)) or \
                    not math.isfinite(value) or abs(value) > 1_000_000:
                raise ValueError("bounded finite numeric coordinate required")

    dp = sum((a - p) ** 2 for a, p in zip(anchor, positive))
    dn = sum((a - n) ** 2 for a, n in zip(anchor, negative))
    return max(0.0, math.sqrt(dp) - math.sqrt(dn) + float(margin))
