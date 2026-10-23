"""Numerically stable softmax for finite real logits."""

import math
from typing import Sequence


def softmax(logits: Sequence[float]) -> list[float]:
    """Return a nonempty probability vector; raise ValueError on invalid data."""
    if len(logits) == 0:
        raise ValueError("softmax requires a nonempty input")
    values = []
    for value in logits:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("logits must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("logits must be representable") from exc
        if not math.isfinite(number):
            raise ValueError("logits must be finite")
        values.append(number)
    maximum = max(values)
    exponentials = [math.exp(value - maximum) for value in values]
    total = sum(exponentials)
    if not math.isfinite(total) or total <= 0:
        raise ValueError("invalid softmax normalizer")
    return [value / total for value in exponentials]
