"""Overflow-safe elementwise logistic sigmoid."""

import math
from typing import Sequence


def sigmoid(values: Sequence[float]) -> list[float]:
    """Map finite real values to [0, 1], preserving vector length."""
    result = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (float, int)):
            raise ValueError("components must be real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("components must be representable") from exc
        if not math.isfinite(x):
            raise ValueError("components must be finite")
        if x >= 0:
            result.append(1.0 / (1.0 + math.exp(-x)))
        else:
            ex = math.exp(x)
            result.append(ex / (1.0 + ex))
    return result
