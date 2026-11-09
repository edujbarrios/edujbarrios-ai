"""Fill four-connected enclosed zero regions in a binary mask."""
from collections import deque


def binary_hole_filling(
    pixels: list[int] | tuple[int, ...], height: int, width: int,
) -> list[int]:
    """Keep border-connected zeros; turn all other zeros to ones."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 64 or not 1 <= width <= 64):
        raise ValueError("dimensions must be integers in 1..64")
    if (not isinstance(pixels, (list, tuple))
            or len(pixels) != height * width
            or any(type(x) is not int or x not in (0, 1) for x in pixels)):
        raise ValueError("expected a flat binary image")
    n = len(pixels)
    visited = [False] * n
    queue = deque()
    for index, pixel in enumerate(pixels):
        row, col = divmod(index, width)
        if (pixel == 0 and (row == 0 or row == height - 1
                            or col == 0 or col == width - 1)):
            visited[index] = True
            queue.append(index)
    while queue:
        index = queue.popleft()
        row, col = divmod(index, width)
        for nr, nc in ((row - 1, col), (row + 1, col),
                       (row, col - 1), (row, col + 1)):
            if 0 <= nr < height and 0 <= nc < width:
                nxt = nr * width + nc
                if pixels[nxt] == 0 and not visited[nxt]:
                    visited[nxt] = True
                    queue.append(nxt)
    return [0 if visited[i] else 1 for i in range(n)]
