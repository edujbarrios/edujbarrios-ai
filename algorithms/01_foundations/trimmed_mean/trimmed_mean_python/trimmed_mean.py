"""Symmetric trimmed mean of finite observations."""
import math


def trimmed_mean(
    values: list[float] | tuple[float, ...], trim_fraction: float
) -> float:
    """Sort, discard floor(n*fraction) at each tail, average retained values."""
    if (isinstance(trim_fraction, bool)
            or not isinstance(trim_fraction, (int, float))):
        raise ValueError("trim fraction must be a real number in [0, 0.5)")
    try:
        fraction = float(trim_fraction)
    except OverflowError as exc:
        raise ValueError("trim fraction must be finite") from exc
    if not math.isfinite(fraction) or not 0 <= fraction < 0.5:
        raise ValueError("trim fraction must be in [0, 0.5)")
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("expected 1..512 observations")
    checked = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("observations must be finite real numbers")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("observation outside finite float range") from exc
        if not math.isfinite(x):
            raise ValueError("observations must be finite")
        checked.append(x)
    checked.sort()
    k = math.floor(len(checked) * fraction)
    retained = checked[k:len(checked) - k]
    result = 0.0
    for value in retained:
        result += value / len(retained)
        if not math.isfinite(result):
            raise ValueError("nonfinite trimmed mean")
    return result
