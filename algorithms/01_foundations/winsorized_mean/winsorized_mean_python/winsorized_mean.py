"""Symmetric winsorized mean of finite observations."""
import math


def winsorized_mean(
    values: list[float] | tuple[float, ...], trim_fraction: float
) -> float:
    """Clamp floor(n*f) observations in each tail, then average all n."""
    if (isinstance(trim_fraction, bool)
            or not isinstance(trim_fraction, (int, float))):
        raise ValueError("trim fraction must be real in [0,0.5)")
    try:
        fraction = float(trim_fraction)
    except OverflowError as exc:
        raise ValueError("trim fraction outside finite float range") from exc
    if not math.isfinite(fraction) or not 0 <= fraction < 0.5:
        raise ValueError("trim fraction must be in [0,0.5)")
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
    n = len(checked)
    k = math.floor(n * fraction)
    low, high = checked[k], checked[n - k - 1]
    result = 0.0
    for i, value in enumerate(checked):
        clipped = low if i < k else high if i >= n - k else value
        result += clipped / n
        if not math.isfinite(result):
            raise ValueError("nonfinite winsorized mean")
    return result
