"""Whole-image linear 8-bit grayscale contrast stretching."""


def grayscale_contrast_stretching(
    pixels: list[int] | tuple[int, ...], height: int, width: int
) -> list[int]:
    """Map observed min/max to 0/255 using integer half-up rounding."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integer values in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height * width
            or any(type(x) is not int or not 0 <= x <= 255 for x in pixels)):
        raise ValueError("expected flat unsigned 8-bit pixel sequence")
    lo, hi = min(pixels), max(pixels)
    span = hi - lo
    if span == 0:
        return list(pixels)
    return [(510 * (x - lo) + span) // (2 * span) for x in pixels]
