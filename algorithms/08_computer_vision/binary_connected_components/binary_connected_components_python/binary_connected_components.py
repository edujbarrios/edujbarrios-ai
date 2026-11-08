"""Deterministic four-connected components for flat binary masks."""
from collections import deque


def binary_connected_components(
    pixels: list[int] | tuple[int, ...], height: int, width: int,
) -> tuple[list[int], int]:
    """Return row-major labels (0 for background) and component count."""
    if (isinstance(height, bool) or not isinstance(height, int)
            or isinstance(width, bool) or not isinstance(width, int)
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integers in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height * width
            or any(type(pixel) is not int or pixel not in (0, 1) for pixel in pixels)):
        raise ValueError("expected a flat binary pixel vector")
    n = height * width
    labels = [0] * n
    count = 0
    for start in range(n):
        if pixels[start] == 0 or labels[start] != 0:
            continue
        count += 1
        labels[start] = count
        queue = deque([start])
        while queue:
            index = queue.popleft()
            row, col = divmod(index, width)
            for nr, nc in ((row - 1, col), (row + 1, col),
                           (row, col - 1), (row, col + 1)):
                if not (0 <= nr < height and 0 <= nc < width):
                    continue
                neighbor = nr * width + nc
                if pixels[neighbor] == 1 and labels[neighbor] == 0:
                    labels[neighbor] = count
                    queue.append(neighbor)
    return labels, count
