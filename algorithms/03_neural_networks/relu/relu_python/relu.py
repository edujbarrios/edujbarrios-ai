"""Elementwise finite rectified-linear activation."""

import math
from typing import Sequence


def relu(values: Sequence[float]) -> list[float]:
    """Return nonnegative vector, preserving positive values."""
    output = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("components must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("component is not representable") from exc
        if not math.isfinite(number):
            raise ValueError("components must be finite")
        output.append(max(0.0, number))
    return output
