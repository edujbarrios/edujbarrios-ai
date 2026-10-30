"""Clip each finite gradient component independently to a symmetric bound."""
import math
from typing import Sequence

def gradient_clip_by_value(gradients: Sequence[float], clip_value: float) -> list[float]:
    """Return clipped gradients; reject invalid input with ValueError."""
    if isinstance(clip_value, bool) or not isinstance(clip_value, (int, float)):
        raise ValueError("clip_value must be finite and positive")
    try:
        bound = float(clip_value)
    except OverflowError as exc:
        raise ValueError("clip_value is not representable") from exc
    if not math.isfinite(bound) or bound <= 0:
        raise ValueError("clip_value must be finite and positive")
    if len(gradients) == 0:
        raise ValueError("gradients must be nonempty")
    values = []
    for value in gradients:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("gradients must be real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("gradient is not representable") from exc
        if not math.isfinite(x):
            raise ValueError("gradients must be finite")
        values.append(x)
    return [min(bound, max(-bound, x)) for x in values]
