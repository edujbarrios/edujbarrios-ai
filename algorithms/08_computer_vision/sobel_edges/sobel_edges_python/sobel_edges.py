"""3x3 Sobel gradient magnitude with replicated image borders."""
import math

KX = ((-1, 0, 1), (-2, 0, 2), (-1, 0, 1))
KY = ((-1, -2, -1), (0, 0, 0), (1, 2, 1))


def sobel_edges(pixels, height, width):
    """Return row-major per-pixel magnitudes after Sobel convolution."""
    for dimension in (height, width):
        if (isinstance(dimension, bool) or not isinstance(dimension, int) or
                not 1 <= dimension <= 64):
            raise ValueError("image dimensions must be integers in 1..64")
    if not isinstance(pixels, (list, tuple)) or len(pixels) != height * width:
        raise ValueError("image must be a flat row-major pixel vector")
    values = []
    for p in pixels:
        if isinstance(p, bool) or not isinstance(p, (int, float)):
            raise ValueError("finite numeric pixels required")
        try:
            x = float(p)
        except OverflowError as exc:
            raise ValueError("unrepresentable pixel") from exc
        if not math.isfinite(x) or abs(x) > 1000:
            raise ValueError("pixel outside [-1000,1000]")
        values.append(x)

    result = []
    for row in range(height):
        for col in range(width):
            gx = gy = 0.0
            for y in range(3):
                for x in range(3):
                    r = max(0, min(height - 1, row + y - 1))
                    c = max(0, min(width - 1, col + x - 1))
                    pixel = values[r * width + c]
                    gx += KX[y][x] * pixel
                    gy += KY[y][x] * pixel
            result.append(math.hypot(gx, gy))
    return result
