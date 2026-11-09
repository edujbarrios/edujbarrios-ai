"""Scale-stable harmonic mean of finite nonnegative numbers."""
import math


def harmonic_mean(values: list[float] | tuple[float, ...]) -> float:
    """Return a harmonic mean without summing unbounded reciprocals."""
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("expected 1..512 nonnegative observations")
    checked = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("expected finite real observations")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("observation outside finite range") from exc
        if not math.isfinite(x) or x < 0.0:
            raise ValueError("expected finite nonnegative observations")
        checked.append(x)
    a = min(checked)
    if a == 0.0:
        return 0.0
    scaled_sum = math.fsum(a / x for x in checked)
    result = a / (scaled_sum / len(checked))
    if not math.isfinite(result):
        raise ValueError("nonfinite harmonic mean")
    return result
