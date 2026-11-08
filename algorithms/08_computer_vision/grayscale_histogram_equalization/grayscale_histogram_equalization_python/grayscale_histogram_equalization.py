"""Deterministic global grayscale histogram equalization."""


def grayscale_histogram_equalization(
    pixels: list[int] | tuple[int, ...], height: int, width: int
) -> list[int]:
    """Return globally equalized 8-bit pixels in row-major order."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integers in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height * width
            or any(type(x) is not int or not 0 <= x <= 255 for x in pixels)):
        raise ValueError("expected flat unsigned 8-bit pixels")
    counts = [0] * 256
    for x in pixels:
        counts[x] += 1
    cumulative = 0
    cdf_min = next(count for count in counts if count)
    denom = len(pixels) - cdf_min
    if denom == 0:
        return list(pixels)
    mapping = [0] * 256
    for value, count in enumerate(counts):
        cumulative += count
        if cumulative > cdf_min:
            mapping[value] = (255 * (cumulative - cdf_min) * 2 + denom) // (2 * denom)
    return [mapping[x] for x in pixels]
