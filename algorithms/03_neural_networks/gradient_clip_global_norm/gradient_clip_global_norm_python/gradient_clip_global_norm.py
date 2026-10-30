"""Overflow-resistant L2 gradient norm clipping without external packages."""
import math
from typing import Sequence


def gradient_clip_global_norm(gradients: Sequence[float], max_norm: float) -> list[float]:
    """Return clipped gradients, rejecting invalid inputs with ValueError."""
    if isinstance(max_norm, bool) or not isinstance(max_norm, (int, float)):
        raise ValueError("max_norm must be finite and positive")
    try:
        bound = float(max_norm)
    except OverflowError as exc:
        raise ValueError("max_norm must be representable") from exc
    if not math.isfinite(bound) or bound <= 0:
        raise ValueError("max_norm must be finite and positive")
    if len(gradients) == 0:
        raise ValueError("gradients must be nonempty")
    values = []
    for value in gradients:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("gradients must be finite real values")
        try:
            converted = float(value)
        except OverflowError as exc:
            raise ValueError("gradients must be representable") from exc
        if not math.isfinite(converted):
            raise ValueError("gradients must be finite")
        values.append(converted)
    maximum = max(abs(x) for x in values)
    if maximum == 0:
        return values
    scaled_norm = math.sqrt(math.fsum((x / maximum) ** 2 for x in values))
    factor = min(1.0, (bound / maximum) / scaled_norm)
    return [x * factor for x in values]
