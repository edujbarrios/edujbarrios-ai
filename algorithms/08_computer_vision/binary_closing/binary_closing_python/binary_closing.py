"""Binary morphological closing with a 3x3 square and zero-padded border."""
from typing import Sequence


def binary_closing(image: Sequence[int], width: int, height: int) -> list[int]:
    """Dilate then erode a row-major binary image, or raise ValueError."""
    if (type(width) is not int or type(height) is not int or
            not 1 <= width <= 64 or not 1 <= height <= 64 or
            len(image) != width * height):
        raise ValueError("width/height must be 1..64 and image length must match")
    if any(type(x) is not int or x not in (0, 1) for x in image):
        raise ValueError("pixels must be integer 0 or 1")
    dilated = [0] * len(image)
    for y in range(height):
        for x in range(width):
            on = 0
            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    yy, xx = y + dy, x + dx
                    if 0 <= yy < height and 0 <= xx < width and image[yy * width + xx] == 1:
                        on = 1
            dilated[y * width + x] = on
    result = [0] * len(image)
    for y in range(height):
        for x in range(width):
            on = 1
            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    yy, xx = y + dy, x + dx
                    if not (0 <= yy < height and 0 <= xx < width) or dilated[yy * width + xx] == 0:
                        on = 0
            result[y * width + x] = on
    return result
