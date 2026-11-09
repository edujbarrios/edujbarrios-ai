"""Type-7 linearly interpolated sample quantile."""
import math


def quantile_linear_interpolation(
    values: list[float] | tuple[float, ...], probability: float
) -> float:
    """Return the finite type-7 quantile of finite observations."""
    if isinstance(probability, bool) or not isinstance(probability, (int, float)):
        raise ValueError("probability must be a real scalar in [0,1]")
    try:
        p = float(probability)
    except OverflowError as exc:
        raise ValueError("probability outside finite range") from exc
    if not math.isfinite(p) or not 0.0 <= p <= 1.0:
        raise ValueError("probability must be finite and in [0,1]")
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("expected 1..512 finite observations")
    checked = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("expected real observations")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("observation outside finite range") from exc
        if not math.isfinite(x):
            raise ValueError("observations must be finite")
        checked.append(x)
    checked.sort()
    h = (len(checked) - 1) * p
    i = math.floor(h)
    f = h - i
    result = checked[i] if f == 0 else (1.0 - f) * checked[i] + f * checked[i+1]
    if not math.isfinite(result):
        raise ValueError("nonfinite sample quantile")
    return result
