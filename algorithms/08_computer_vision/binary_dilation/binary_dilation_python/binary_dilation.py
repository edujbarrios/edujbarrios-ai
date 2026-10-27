"""3x3 square binary morphological dilation with zero-padded edges."""


def binary_dilation(pixels, height, width):
    """Return row-major 0/1 mask after one dilation iteration."""
    for d in (height, width):
        if isinstance(d, bool) or not isinstance(d, int) or not 1 <= d <= 64:
            raise ValueError("dimensions must be integers in 1..64")
    if not isinstance(pixels, (list, tuple)) or len(pixels) != height*width:
        raise ValueError("binary image must be a flat H*W array")
    if any(isinstance(x, bool) or not isinstance(x, int) or x not in (0, 1)
           for x in pixels):
        raise ValueError("binary pixels must be 0 or 1 integers")
    out = []
    for r in range(height):
        for c in range(width):
            hit = 0
            for dr in (-1, 0, 1):
                for dc in (-1, 0, 1):
                    y, x = r+dr, c+dc
                    if 0 <= y < height and 0 <= x < width and pixels[y*width+x]:
                        hit = 1
            out.append(hit)
    return out
