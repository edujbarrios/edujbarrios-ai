"""Fixed 3x3 Gaussian-approximation blur with replicated borders."""
import math


def gaussian_blur(pixels, height, width):
    """Filter a flat row-major grayscale image, preserving dimensions."""
    if (isinstance(height, bool) or not isinstance(height, int)
            or not 1 <= height <= 64
            or isinstance(width, bool) or not isinstance(width, int)
            or not 1 <= width <= 64):
        raise ValueError("dimensions must be integers in 1..64")
    if not isinstance(pixels, (list, tuple)) or len(pixels) != height * width:
        raise ValueError("incorrect flat image size")
    values = []
    for pixel in pixels:
        if isinstance(pixel, bool) or not isinstance(pixel, (int, float)):
            raise ValueError("pixels must be real numbers")
        try:
            value = float(pixel)
        except OverflowError as exc:
            raise ValueError("pixel cannot be represented") from exc
        if not math.isfinite(value) or abs(value) > 1_000_000:
            raise ValueError("pixel outside allowed range")
        values.append(value)

    out = []
    for row in range(height):
        for col in range(width):
            window = [
                values[min(height - 1, max(0, row + dr)) * width
                       + min(width - 1, max(0, col + dc))]
                for dr in (-1, 0, 1) for dc in (-1, 0, 1)
            ]
            out.append(sum(value * weight for value, weight in zip(window, (1, 2, 1, 2, 4, 2, 1, 2, 1))) / 16.0)
    return out
