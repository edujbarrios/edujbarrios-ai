"""Sample-weighted mean absolute regression error."""
import math
from typing import Sequence

def weighted_mean_absolute_error(targets: Sequence[float], predictions: Sequence[float],
                                 weights: Sequence[float]) -> float:
    """Return WMAE; raise ValueError for invalid data and zero weight sum."""
    n = len(targets)
    if n == 0 or n > 4096 or len(predictions) != n or len(weights) != n:
        raise ValueError("three vectors must have equal nonzero length <= 4096")
    weighted_sum = 0.0
    total_weight = 0.0
    for raw_y, raw_p, raw_w in zip(targets, predictions, weights):
        if any(isinstance(x, bool) or not isinstance(x, (int, float))
               for x in (raw_y, raw_p, raw_w)):
            raise ValueError("inputs must be finite real numbers")
        try:
            y, p, w = float(raw_y), float(raw_p), float(raw_w)
        except OverflowError as exc:
            raise ValueError("input is not representable") from exc
        if not (math.isfinite(y) and math.isfinite(p) and math.isfinite(w) and
                abs(y) <= 1e6 and abs(p) <= 1e6 and 0 <= w <= 1e6):
            raise ValueError("invalid target, prediction or weight")
        weighted_sum += w * abs(y - p)
        total_weight += w
    if total_weight <= 0:
        raise ValueError("sum of sample weights must be positive")
    return weighted_sum / total_weight
