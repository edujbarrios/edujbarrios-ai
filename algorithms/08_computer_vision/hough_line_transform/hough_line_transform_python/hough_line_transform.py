"""Sparse rho-theta Hough voting for binary image edge maps."""

from collections import Counter
import math


def _bounded_real(value, name, lower, upper, inclusive_upper=True):
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"{name} must be a bounded finite real")
    try:
        number = float(value)
    except (ValueError, OverflowError):
        raise ValueError(f"{name} must be a bounded finite real") from None
    if (not math.isfinite(number) or number < lower or
            (number > upper if inclusive_upper else number >= upper)):
        raise ValueError(f"{name} must be a bounded finite real")
    return number


def hough_line_transform(edges, angles=(0, 45, 90, 135),
                         rho_step=1.0, top_k=5):
    """Return top (normal angle, rho, votes) sorted by vote count and ties."""
    if not isinstance(edges, (list, tuple)) or not 1 <= len(edges) <= 128:
        raise ValueError("edges must be a binary image with 1..128 rows")
    if not isinstance(edges[0], (list, tuple)) or not 1 <= len(edges[0]) <= 128:
        raise ValueError("edge image width must be 1..128")
    width = len(edges[0])
    for row in edges:
        if not isinstance(row, (list, tuple)) or len(row) != width:
            raise ValueError("all image rows must be the same width")
        if any(type(pixel) is not int or pixel not in (0, 1) for pixel in row):
            raise ValueError("image pixels must be integer zeros or ones")

    if not isinstance(angles, (tuple, list)) or not 1 <= len(angles) <= 180:
        raise ValueError("angles must contain 1..180 distinct degrees")
    theta = [_bounded_real(value, "angle", 0.0, 180.0, False)
             for value in angles]
    if len(set(theta)) != len(theta):
        raise ValueError("angles must be distinct")

    step = _bounded_real(rho_step, "rho_step", 0.001, 1000.0)
    if isinstance(top_k, bool) or not isinstance(top_k, int) or not 1 <= top_k <= 1024:
        raise ValueError("top_k must be an integer in 1..1024")

    trig = [(math.cos(math.radians(angle)), math.sin(math.radians(angle)))
            for angle in theta]
    accumulator = Counter()
    for y, row in enumerate(edges):
        for x, pixel in enumerate(row):
            if pixel:
                for index, (cosine, sine) in enumerate(trig):
                    rho_bin = round((x * cosine + y * sine) / step)
                    accumulator[index, rho_bin] += 1

    ordered = sorted(accumulator.items(),
                     key=lambda item: (-item[1], item[0][0], item[0][1]))
    return [(theta[index], rho_bin * step, votes)
            for ((index, rho_bin), votes) in ordered[:top_k]]
