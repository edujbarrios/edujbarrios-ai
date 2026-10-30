"""Binary morphological opening with a fixed 3x3 kernel and zero border."""
from typing import Sequence

def binary_opening(image: Sequence[int], width: int, height: int) -> list[int]:
    """Erode then dilate a flattened image; invalid input raises ValueError."""
    if (type(width) is not int or type(height) is not int or
            not 1 <= width <= 64 or not 1 <= height <= 64 or
            len(image) != width * height):
        raise ValueError("width/height must be 1..64 and image have matching length")
    if any(type(v) is not int or v not in (0, 1) for v in image):
        raise ValueError("pixels must be integers 0 or 1")
    eroded = [0] * len(image)
    for y in range(height):
        for x in range(width):
            keep = 1
            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    ny, nx = y + dy, x + dx
                    if not (0 <= ny < height and 0 <= nx < width) or image[ny * width + nx] == 0:
                        keep = 0
            eroded[y * width + x] = keep
    output = [0] * len(image)
    for y in range(height):
        for x in range(width):
            keep = 0
            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    ny, nx = y + dy, x + dx
                    if 0 <= ny < height and 0 <= nx < width and eroded[ny * width + nx]:
                        keep = 1
            output[y * width + x] = keep
    return output
