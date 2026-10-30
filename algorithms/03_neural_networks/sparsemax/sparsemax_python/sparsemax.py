"""Forward sparsemax projection of finite logits onto the probability simplex."""

import math
from typing import Sequence


def sparsemax(logits: Sequence[float]) -> list[float]:
    """Return sparse probabilities; raise ValueError for invalid input."""
    if len(logits) == 0:
        raise ValueError("logits must be nonempty")
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
    shifted = [v - maximum for v in values]
    sorted_logits = sorted(shifted, reverse=True)
    prefix = 0.0
    threshold = 0.0
    for k, value in enumerate(sorted_logits, 1):
        prefix += value
        if 1.0 + k * value > prefix:
            threshold = (prefix - 1.0) / k
    return [max(0.0, value - threshold) for value in shifted]
