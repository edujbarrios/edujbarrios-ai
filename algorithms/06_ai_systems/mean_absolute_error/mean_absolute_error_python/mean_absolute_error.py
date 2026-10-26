"""Mean absolute error for finite real-valued regression observations."""

import math
from typing import Sequence


def mean_absolute_error(truth: Sequence[float], predictions: Sequence[float]) -> float:
    """Return mean absolute residual, rejecting nonfinite data and overflow."""
    if not isinstance(truth, (list, tuple)) or not isinstance(predictions, (list, tuple)):
        raise ValueError("numeric vectors must be lists or tuples")
    if not truth or len(truth) != len(predictions):
        raise ValueError("nonempty, aligned vectors are required")
    n = len(truth)
    average = 0.0
    for actual, predicted in zip(truth, predictions):
        if (isinstance(actual, bool) or isinstance(predicted, bool)
                or not isinstance(actual, (int, float))
                or not isinstance(predicted, (int, float))):
            raise ValueError("observations must be real numbers")
        try:
            a, p = float(actual), float(predicted)
        except OverflowError as exc:
            raise ValueError("observations must be finite") from exc
        if not math.isfinite(a) or not math.isfinite(p):
            raise ValueError("observations must be finite")
        residual = a - p
        if not math.isfinite(residual):
            raise ValueError("residual overflow")
        average += abs(residual) / n
        if not math.isfinite(average):
            raise ValueError("average overflow")
    return average
