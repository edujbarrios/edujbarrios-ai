"""Scaled Minkowski L-p distance for finite p>=1."""
import math


def minkowski_distance(a, b, p=2.0):
    if isinstance(p, bool) or not isinstance(p, (float, int)):
        raise ValueError("finite p >= 1 required")
    try:
        exponent = float(p)
    except OverflowError as exc:
        raise ValueError("unrepresentable exponent") from exc
    if not math.isfinite(exponent) or exponent < 1:
        raise ValueError("finite p >= 1 required")

    def check(x):
        if not isinstance(x, (list, tuple)) or not x:
            raise ValueError("nonempty real vector required")
        out = []
        for item in x:
            if isinstance(item, bool) or not isinstance(item, (float, int)):
                raise ValueError("finite real coordinates required")
            try:
                value = float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable coordinate") from exc
            if not math.isfinite(value):
                raise ValueError("finite real coordinates required")
            out.append(value)
        return out

    x, y = check(a), check(b)
    if len(x) != len(y):
        raise ValueError("mismatched vector lengths")
    deltas = [abs(i - j) for i, j in zip(x, y)]
    if not all(math.isfinite(d) for d in deltas):
        raise ValueError("unrepresentable difference")
    scale = max(deltas)
    if scale == 0:
        return 0.0
    power_sum = math.fsum((d / scale) ** exponent for d in deltas)
    answer = scale * power_sum ** (1 / exponent)
    if not math.isfinite(answer):
        raise ValueError("unrepresentable distance")
    return answer
