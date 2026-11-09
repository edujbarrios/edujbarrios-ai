"""Scale-stable root mean square magnitude."""
import math


def root_mean_square(values: list[float] | tuple[float, ...]) -> float:
    """Return nonnegative RMS, scaling by max(abs(x)) before squaring."""
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("expected 1..512 observations")
    checked = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("expected finite real observations")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("observation outside finite range") from exc
        if not math.isfinite(x):
            raise ValueError("observations must be finite")
        checked.append(x)
    scale = max(abs(x) for x in checked)
    if scale == 0:
        return 0.0
    normalized_squares = math.fsum((x / scale) ** 2 for x in checked)
    result = scale * math.sqrt(normalized_squares / len(checked))
    if not math.isfinite(result):
        raise ValueError("nonfinite RMS")
    return result
