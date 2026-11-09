"""Horizontal and vertical binary projection profiles of a row-major image."""


def binary_projection_profiles(
    pixels: list[int] | tuple[int, ...], height: int, width: int,
) -> list[int]:
    """Return H row counts followed by W column counts."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integer values 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height*width
            or any(type(x) is not int or x not in (0, 1) for x in pixels)):
        raise ValueError("expected flat binary mask with exactly H*W values")
    output = [0]*(height+width)
    for i, pixel in enumerate(pixels):
        if pixel:
            row, col = divmod(i,width)
            output[row] += 1
            output[height+col] += 1
    return output
