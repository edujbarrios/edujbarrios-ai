"""Numerically stable Softplus activation and analytical derivative."""
import math
from typing import Sequence


def softplus(inputs: Sequence[float]) -> tuple[list[float], list[float]]:
    """Return (values, derivatives) for a nonempty finite real vector."""
    if not isinstance(inputs, (list, tuple)) or not inputs:
        raise ValueError("nonempty real vector required")
    numbers = []
    for item in inputs:
        if isinstance(item, bool) or not isinstance(item, (int, float)):
            raise ValueError("finite real values required")
        try:
            x = float(item)
        except OverflowError as exc:
            raise ValueError("unrepresentable real value") from exc
        if not math.isfinite(x):
            raise ValueError("finite real values required")
        numbers.append(x)
    values, derivatives = [], []
    for x in numbers:
        values.append(max(x, 0.0) + math.log1p(math.exp(-abs(x))))
        if x >= 0:
            derivative = 1.0 / (1.0 + math.exp(-x))
        else:
            z = math.exp(x)
            derivative = z / (1.0 + z)
        derivatives.append(derivative)
    return values, derivatives
