"""Coefficient of determination, including finite constant-target handling."""
import math
from typing import Sequence


def r_squared_score(actual: Sequence[float], predicted: Sequence[float]) -> float:
    """Return finite R² for at least two paired real observations."""
    def validate(values: Sequence[float]) -> list[float]:
        if not isinstance(values, (list, tuple)) or len(values) < 2:
            raise ValueError("at least two real observations required")
        items = []
        for item in values:
            if isinstance(item, bool) or not isinstance(item, (int, float)):
                raise ValueError("finite real observations required")
            try:
                number = float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable observation") from exc
            if not math.isfinite(number):
                raise ValueError("finite real observations required")
            items.append(number)
        return items

    y, pred = validate(actual), validate(predicted)
    if len(y) != len(pred):
        raise ValueError("actual and predicted lengths must match")
    if all(value == y[0] for value in y):
        return 1.0 if all(a == b for a, b in zip(y, pred)) else 0.0
    target_scale = max(abs(value) for value in y)
    residual_scale = max(target_scale, max(abs(value) for value in pred))
    baseline = y[0] / target_scale
    offsets = [value / target_scale - baseline for value in y]
    centered_mean = math.fsum(offsets) / len(offsets)
    variation = math.fsum((value - centered_mean) ** 2 for value in offsets)
    if variation == 0.0 or not math.isfinite(variation):
        raise ValueError("target variation not representable")
    squared_error = math.fsum(
        (a / residual_scale - b / residual_scale) ** 2
        for a, b in zip(y, pred)
    )
    magnitude = residual_scale / target_scale
    ratio = squared_error / variation * magnitude * magnitude
    score = 1.0 - ratio
    if not math.isfinite(score):
        raise ValueError("unrepresentable R-squared")
    return score
