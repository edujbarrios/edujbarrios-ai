"""Interquartile range from type-7 linearly interpolated quantiles."""
import math


def interquartile_range(values: list[float] | tuple[float, ...]) -> float:
    """Return Q(0.75)-Q(0.25), with R-default type-7 quartiles."""
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("expected 1..512 finite observations")
    checked = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("observations must be real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("observation outside finite float range") from exc
        if not math.isfinite(x):
            raise ValueError("observations must be finite")
        checked.append(x)
    checked.sort()

    def quantile(p: float) -> float:
        h = (len(checked) - 1) * p
        i = int(h)
        fraction = h - i
        if fraction == 0.0:
            return checked[i]
        return (1.0 - fraction) * checked[i] + fraction * checked[i + 1]

    result = quantile(0.75) - quantile(0.25)
    if not math.isfinite(result):
        raise ValueError("interquartile range is nonfinite")
    return max(0.0, result)
