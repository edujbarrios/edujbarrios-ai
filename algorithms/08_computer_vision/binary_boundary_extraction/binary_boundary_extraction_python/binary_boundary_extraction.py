"""Extract foreground pixels adjacent to background under four connectivity."""


def binary_boundary_extraction(
    pixels: list[int] | tuple[int, ...], height: int, width: int,
) -> list[int]:
    """Mark foreground pixels with a missing four-neighbor foreground cell."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integer values in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height*width
            or any(type(x) is not int or x not in (0, 1) for x in pixels)):
        raise ValueError("expected flat 0/1 pixel sequence")
    result = [0]*(height*width)
    for i, value in enumerate(pixels):
        if value == 0:
            continue
        row, col = divmod(i,width)
        if (row == 0 or row+1 == height or col == 0 or col+1 == width
                or pixels[i-width] == 0 or pixels[i+width] == 0
                or pixels[i-1] == 0 or pixels[i+1] == 0):
            result[i] = 1
    return result
