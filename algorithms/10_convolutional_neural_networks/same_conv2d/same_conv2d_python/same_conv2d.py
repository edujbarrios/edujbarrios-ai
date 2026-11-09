"""Single-channel same-padded CNN cross-correlation with odd-sized kernels."""
import math


def same_conv2d(image, height, width, kernel, kh, kw):
    """Return row-major H×W result for stride-1 zero-padded correlation."""
    if (type(height) is not int or type(width) is not int
            or not 1 <= height <= 32 or not 1 <= width <= 32
            or type(kh) is not int or type(kw) is not int
            or kh not in (1, 3, 5) or kw not in (1, 3, 5)):
        raise ValueError("image shape 1..32; odd kernel dimensions 1,3,5")
    if (not isinstance(image, (list, tuple)) or len(image) != height*width
            or not isinstance(kernel, (list, tuple)) or len(kernel) != kh*kw):
        raise ValueError("expected flat image and kernel of declared shapes")

    def check(values):
        out = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("expected finite real pixel and kernel values")
            try:
                x = float(value)
            except OverflowError as exc:
                raise ValueError("value cannot be represented as a float") from exc
            if not math.isfinite(x) or abs(x) > 1000.0:
                raise ValueError("values must be finite in [-1000,1000]")
            out.append(x)
        return out

    x = check(image)
    k = check(kernel)
    result = [0.0] * (height*width)
    row_pad, col_pad = kh//2, kw//2
    for row in range(height):
        for col in range(width):
            total = 0.0
            for ky in range(kh):
                rr = row + ky - row_pad
                if not 0 <= rr < height:
                    continue
                for kx in range(kw):
                    cc = col + kx - col_pad
                    if 0 <= cc < width:
                        total += x[rr*width+cc] * k[ky*kw+kx]
            result[row*width+col] = total
    return result
