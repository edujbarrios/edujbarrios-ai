"""Fixed 3x3 binary erosion with zero exterior padding."""


def binary_erosion(pixels, height, width):
    """Return an H*W flat integer mask after erosion."""
    for dimension in (height, width):
        if isinstance(dimension, bool) or not isinstance(dimension, int) or not 1 <= dimension <= 64:
            raise ValueError("dimensions must be integers 1..64")
    if not isinstance(pixels, (list, tuple)) or len(pixels) != height*width:
        raise ValueError("expected a flat H*W input")
    if any(isinstance(x, bool) or not isinstance(x, int) or x not in (0,1) for x in pixels):
        raise ValueError("input values must be integers 0 or 1")
    result = []
    for r in range(height):
        for c in range(width):
            hit = 1
            for dr in (-1, 0, 1):
                for dc in (-1, 0, 1):
                    y, x = r+dr, c+dc
                    if not (0 <= y < height and 0 <= x < width and pixels[y*width+x]):
                        hit = 0
            result.append(hit)
    return result
