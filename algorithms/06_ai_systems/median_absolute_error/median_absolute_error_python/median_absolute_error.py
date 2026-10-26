"""Median absolute regression error for finite, equally sized vectors."""
import math
from typing import Sequence


def median_absolute_error(actual: Sequence[float], predicted: Sequence[float]) -> float:
    """Return median absolute residual, rejecting overflow and invalid input."""
    def checked(data: Sequence[float]) -> list[float]:
        if not isinstance(data, (tuple, list)) or not data:
            raise ValueError("nonempty real vector required")
        output = []
        for item in data:
            if isinstance(item, bool) or not isinstance(item, (float, int)):
                raise ValueError("finite real observations required")
            try:
                number = float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable observation") from exc
            if not math.isfinite(number):
                raise ValueError("finite real observations required")
            output.append(number)
        return output

    y, prediction = checked(actual), checked(predicted)
    if len(y) != len(prediction):
        raise ValueError("vector lengths must match")
    errors = []
    for a, b in zip(y, prediction):
        error = abs(a - b)
        if not math.isfinite(error):
            raise ValueError("absolute residual overflow")
        errors.append(error)
    errors.sort()
    mid = len(errors) // 2
    if len(errors) % 2:
        return errors[mid]
    lower, upper = errors[mid - 1], errors[mid]
    return lower + (upper - lower) / 2.0
