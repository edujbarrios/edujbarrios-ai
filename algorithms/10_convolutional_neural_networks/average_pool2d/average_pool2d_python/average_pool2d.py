"""Disjoint 2x2 stride-2 average pooling."""
import math


def average_pool2d(image, height, width):
    if (type(height) is not int or type(width) is not int
            or not 2 <= height <= 32 or not 2 <= width <= 32):
        raise ValueError("dimensions must be integers in 2..32")
    if not isinstance(image, (list, tuple)) or len(image) != height * width:
        raise ValueError("expected flat H*W image")
    checked = []
    for v in image:
        if isinstance(v, bool) or not isinstance(v, (int, float)):
            raise ValueError("image values must be real")
        try:
            x = float(v)
        except OverflowError as exc:
            raise ValueError("unrepresentable image value") from exc
        if not math.isfinite(x) or abs(x) > 1000:
            raise ValueError("image value outside finite range")
        checked.append(x)
    out = []
    for r in range(height // 2):
        for c in range(width // 2):
            i = 2*r*width + 2*c
            out.append((checked[i] + checked[i+1] +
                        checked[i+width] + checked[i+width+1]) / 4.0)
    return out
