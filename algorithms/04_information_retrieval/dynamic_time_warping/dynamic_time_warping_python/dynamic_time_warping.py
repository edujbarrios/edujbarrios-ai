"""Dynamic time warping with absolute local cost and optional diagonal band."""

import math


def _series(values, name):
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError(f"{name} must contain 1..512 numeric observations")
    result = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError(f"{name} must contain bounded finite real values")
        try:
            number = float(value)
        except (ValueError, OverflowError):
            raise ValueError(f"{name} must contain bounded finite real values") from None
        if not math.isfinite(number) or abs(number) > 1_000_000:
            raise ValueError(f"{name} must contain bounded finite real values")
        result.append(number)
    return result


def dynamic_time_warping(a, b, window=None):
    """Return the minimum summed absolute alignment cost (no path output).

    A window, when supplied, bounds |i-j| in the 1-based DP indices.
    """
    left, right = _series(a, "a"), _series(b, "b")
    if len(left) < len(right):
        left, right = right, left  # O(min(n,m)) row storage.
    n, m = len(left), len(right)
    if window is None:
        window = n
    elif (isinstance(window, bool) or not isinstance(window, int)
          or not n - m <= window <= 512):
        raise ValueError("window must be an integer in [|n-m|, 512]")

    previous = [math.inf] * (m + 1)
    previous[0] = 0.0
    for i in range(1, n + 1):
        current = [math.inf] * (m + 1)
        for j in range(max(1, i - window), min(m, i + window) + 1):
            cost = abs(left[i - 1] - right[j - 1])
            current[j] = cost + min(previous[j], current[j - 1],
                                    previous[j - 1])
        previous = current
    result = previous[m]
    if not math.isfinite(result):
        raise ValueError("no alignment found in the requested window")
    return result
