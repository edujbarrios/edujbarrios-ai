"""Count four-neighbor exposed edges of foreground binary pixels."""


def binary_perimeter(
    pixels: list[int] | tuple[int, ...], height: int, width: int,
) -> int:
    """Return perimeter in unit grid edges, treating outside as background."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integers in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height * width
            or any(type(v) is not int or v not in (0, 1) for v in pixels)):
        raise ValueError("expected a flat binary mask")
    total = 0
    for index, value in enumerate(pixels):
        if value == 0:
            continue
        row, col = divmod(index, width)
        if row == 0 or pixels[index - width] == 0:
            total += 1
        if row + 1 == height or pixels[index + width] == 0:
            total += 1
        if col == 0 or pixels[index - 1] == 0:
            total += 1
        if col + 1 == width or pixels[index + 1] == 0:
            total += 1
    return total
