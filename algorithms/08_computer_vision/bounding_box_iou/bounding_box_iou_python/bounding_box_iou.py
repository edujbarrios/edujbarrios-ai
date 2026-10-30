"""Axis-aligned bounding box intersection over union."""
import math
from typing import Sequence


def bounding_box_iou(a: Sequence[float], b: Sequence[float]) -> float:
    """Return bounding box IoU; reject malformed coordinates with ValueError."""
    def validate(box: Sequence[float]) -> tuple[float, ...]:
        if len(box) != 4:
            raise ValueError("each box must contain four coordinates")
        values = []
        for value in box:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("coordinates must be finite real numbers")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("coordinate is not representable") from exc
            if not math.isfinite(number) or abs(number) > 1_000_000:
                raise ValueError("coordinates must be finite and within bounds")
            values.append(number)
        if values[0] >= values[2] or values[1] >= values[3]:
            raise ValueError("boxes must have positive width and height")
        return tuple(values)

    a = validate(a)
    b = validate(b)
    width = max(0.0, min(a[2], b[2]) - max(a[0], b[0]))
    height = max(0.0, min(a[3], b[3]) - max(a[1], b[1]))
    overlap = width * height
    area_a = (a[2] - a[0]) * (a[3] - a[1])
    area_b = (b[2] - b[0]) * (b[3] - b[1])
    return overlap / (area_a + area_b - overlap)
