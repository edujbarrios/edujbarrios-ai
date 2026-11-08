"""Inclusive row-major integral image for unsigned 8-bit pixels."""


def integral_image(pixels: list[int] | tuple[int, ...],
                   height: int, width: int) -> list[int]:
    """Compute the inclusive summed-area table in row-major order."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integers in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height * width
            or any(type(pixel) is not int or not 0 <= pixel <= 255
                   for pixel in pixels)):
        raise ValueError("expected a flat unsigned 8-bit image")
    result = [0] * (height * width)
    for row in range(height):
        row_sum = 0
        for col in range(width):
            index = row * width + col
            row_sum += pixels[index]
            result[index] = row_sum + (result[index - width] if row else 0)
    return result
