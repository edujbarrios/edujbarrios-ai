"""Geometric mean of nonnegative finite observations using logarithms."""

import math


def geometric_mean(values: list[float] | tuple[float, ...]) -> float:
    """Return geometric mean of 1..512 finite nonnegative real values."""
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("expected 1..512 real observations")
    checked = []
    has_zero = False
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("observations must be real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("observation cannot be represented as finite float") from exc
        if not math.isfinite(x) or x < 0:
            raise ValueError("observations must be finite and nonnegative")
        checked.append(x)
        has_zero |= x == 0.0
    if has_zero:
        return 0.0
    log_sum = 0.0
    for x in checked:
        log_sum += math.log(x)
        if not math.isfinite(log_sum):
            raise ValueError("nonfinite logarithm sum")
    result = math.exp(log_sum / len(checked))
    if not math.isfinite(result):
        raise ValueError("nonfinite geometric mean")
    return result
