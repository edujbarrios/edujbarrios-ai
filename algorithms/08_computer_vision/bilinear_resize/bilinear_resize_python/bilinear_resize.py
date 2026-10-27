"""Half-pixel-center bilinear interpolation of a flat grayscale image."""
import math


def bilinear_resize(pixels, height, width, output_height, output_width):
    """Return resized pixels as a row-major list of doubles."""
    for d in (height, width, output_height, output_width):
        if isinstance(d, bool) or not isinstance(d, int) or not 1 <= d <= 64:
            raise ValueError("all dimensions must be integers in 1..64")
    if not isinstance(pixels, (list, tuple)) or len(pixels) != height * width:
        raise ValueError("flat image has incorrect size")
    values = []
    for value in pixels:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("finite numeric pixels required")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable pixel") from exc
        if not math.isfinite(number) or abs(number) > 1_000_000:
            raise ValueError("pixel outside allowed range")
        values.append(number)

    def coordinate(index, source_size, destination_size):
        position = (index + 0.5) * source_size / destination_size - 0.5
        return max(0.0, min(float(source_size - 1), position))

    out = []
    for r in range(output_height):
        y = coordinate(r, height, output_height)
        y0 = int(math.floor(y))
        y1 = min(y0 + 1, height - 1)
        wy = y - y0
        for c in range(output_width):
            x = coordinate(c, width, output_width)
            x0 = int(math.floor(x))
            x1 = min(x0 + 1, width - 1)
            wx = x - x0
            a = (1 - wx)*values[y0*width+x0] + wx*values[y0*width+x1]
            b = (1 - wx)*values[y1*width+x0] + wx*values[y1*width+x1]
            out.append((1 - wy)*a + wy*b)
    return out
