"""Deterministic Top-K indices for finite numerical scores."""
import math
from typing import Sequence

def top_k(scores: Sequence[float], k: int) -> list[int]:
    """Return indices ordered by descending score then ascending index."""
    if isinstance(k, bool) or not isinstance(k, int) or k < 1:
        raise ValueError("k must be a positive integer")
    if not scores or k > len(scores):
        raise ValueError("scores must be nonempty and contain at least k items")
    converted = []
    for value in scores:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("scores must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("scores must be representable") from exc
        if not math.isfinite(number):
            raise ValueError("scores must be finite")
        converted.append(number)
    return sorted(range(len(converted)), key=lambda i: (-converted[i], i))[:k]
