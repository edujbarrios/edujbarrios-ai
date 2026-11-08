"""Unscaled median absolute deviation of finite real observations."""
import math


def _median_sorted(items: list[float]) -> float:
    n = len(items)
    if n % 2:
        return items[n // 2]
    left, right = items[n // 2 - 1], items[n // 2]
    if left <= 0.0 <= right:
        return left / 2.0 + right / 2.0
    return left + (right - left) / 2.0


def median_absolute_deviation(values: list[float] | tuple[float, ...]) -> float:
    """Return median(|x - median(x)|) without a scale correction."""
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("expected 1 to 512 numeric observations")
    checked = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("values must be finite real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("value out of finite float range") from exc
        if not math.isfinite(x):
            raise ValueError("values must be finite")
        checked.append(x)
    center = _median_sorted(sorted(checked))
    deviations = [abs(x - center) for x in checked]
    if not all(math.isfinite(x) for x in deviations):
        raise ValueError("nonfinite deviation")
    result = _median_sorted(sorted(deviations))
    if not math.isfinite(result):
        raise ValueError("nonfinite median absolute deviation")
    return result
