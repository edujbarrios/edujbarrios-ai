"""Numerically stable multiclass cross-entropy computed from logits."""

import math
from typing import Sequence


def cross_entropy(logits: Sequence[float], target: int) -> float:
    """Return loss for one zero-based class index."""
    if len(logits) == 0:
        raise ValueError("logits must be nonempty")
    if isinstance(target, bool) or not isinstance(target, int) or not 0 <= target < len(logits):
        raise ValueError("target must be zero-based integer index")
    values = []
    for value in logits:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("logits must be real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("logits must be representable") from exc
        if not math.isfinite(x):
            raise ValueError("logits must be finite")
        values.append(x)
    maximum = max(values)
    total = sum(math.exp(v - maximum) for v in values)
    delta = values[target] - maximum
    result = math.log(total) - delta
    if not all(math.isfinite(v) for v in (total, delta, result)):
        raise ValueError("cross entropy not representable")
    return max(0.0, result)
