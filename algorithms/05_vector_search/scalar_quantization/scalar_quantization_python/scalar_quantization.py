"""Uniform 8-bit scalar quantization with explicit fixed range."""
import math
from typing import Sequence

def scalar_quantization(values: Sequence[float], minimum: float,
                        maximum: float) -> list[int]:
    """Return a list of 8-bit unsigned bins for finite real inputs."""
    if any(isinstance(v, bool) or not isinstance(v, (float, int))
           for v in (minimum, maximum)):
        raise ValueError("range endpoints must be finite real numbers")
    try:
        low, high = float(minimum), float(maximum)
    except OverflowError as exc:
        raise ValueError("range endpoints must be representable") from exc
    span = high - low
    if not all(math.isfinite(v) for v in (low, high, span)) or span <= 0:
        raise ValueError("expected finite minimum < maximum and finite span")
    converted = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (float, int)):
            raise ValueError("input must contain real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("input must be representable") from exc
        if not math.isfinite(number):
            raise ValueError("input must contain finite numbers")
        converted.append(number)
    output = []
    for number in converted:
        if number <= low:
            output.append(0)
        elif number >= high:
            output.append(255)
        else:
            scaled = 255.0 * ((number - low) / span)
            output.append(min(255, math.floor(scaled + 0.5)))
    return output
