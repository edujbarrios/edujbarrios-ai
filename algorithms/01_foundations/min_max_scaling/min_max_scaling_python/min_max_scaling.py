"""One-feature min-max scaling on finite, nonconstant real values."""
import math
from typing import Sequence


def min_max_scaling(values: Sequence[float]) -> list[float]:
    """Return a new vector scaled to [0,1] using observed extrema."""
    if not values:
        raise ValueError("input must be nonempty")
    numbers = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("input must contain real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("values must be representable") from exc
        if not math.isfinite(number):
            raise ValueError("input must contain finite values")
        numbers.append(number)
    low, high = min(numbers), max(numbers)
    span = high - low
    if not math.isfinite(span) or span <= 0:
        raise ValueError("input must be nonconstant with representable range")
    output = []
    for number in numbers:
        if number == low:
            output.append(0.0)
        elif number == high:
            output.append(1.0)
        else:
            result = (number - low) / span
            if not math.isfinite(result):
                raise ValueError("normalization overflow")
            output.append(min(1.0, max(0.0, result)))
    return output
