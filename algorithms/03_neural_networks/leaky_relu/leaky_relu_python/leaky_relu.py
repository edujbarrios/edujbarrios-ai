"""Reference Leaky ReLU, without neural-network frameworks."""
import math
from typing import Sequence

def leaky_relu(values: Sequence[float], alpha: float) -> list[float]:
    """Apply x if nonnegative, alpha*x otherwise, for 0 < alpha < 1."""
    if isinstance(alpha, bool) or not isinstance(alpha, (int, float)):
        raise ValueError("alpha must be finite and in (0, 1)")
    try:
        slope = float(alpha)
    except OverflowError as exc:
        raise ValueError("alpha must be finite") from exc
    if not math.isfinite(slope) or not 0.0 < slope < 1.0:
        raise ValueError("alpha must be finite and in (0, 1)")
    output = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("components must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("components must be finite") from exc
        if not math.isfinite(number):
            raise ValueError("components must be finite")
        output.append(number if number >= 0.0 else number * slope)
    return output
