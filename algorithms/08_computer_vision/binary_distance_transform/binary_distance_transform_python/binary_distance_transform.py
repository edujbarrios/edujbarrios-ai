"""Four-neighbor Manhattan distance to the nearest zero in a binary mask."""
from collections import deque


def binary_distance_transform(
    pixels: list[int] | tuple[int, ...], height: int, width: int,
) -> list[int]:
    """Return row-major 4-connected distances; H+W if no zero exists."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integers in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height * width
            or any(type(pixel) is not int or pixel not in (0, 1)
                   for pixel in pixels)):
        raise ValueError("expected flat binary pixel vector")
    sentinel = height + width
    distance = [sentinel] * (height * width)
    queue = deque()
    for index, pixel in enumerate(pixels):
        if pixel == 0:
            distance[index] = 0
            queue.append(index)
    while queue:
        index = queue.popleft()
        row, col = divmod(index, width)
        next_distance = distance[index] + 1
        for nr, nc in ((row - 1, col), (row + 1, col),
                       (row, col - 1), (row, col + 1)):
            if 0 <= nr < height and 0 <= nc < width:
                neighbor = nr * width + nc
                if distance[neighbor] == sentinel:
                    distance[neighbor] = next_distance
                    queue.append(neighbor)
    return distance
