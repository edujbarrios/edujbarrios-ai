"""Class-agnostic score-sorted bounding-box non-maximum suppression."""
import math


def non_max_suppression(boxes, threshold):
    """Return selected zero-based indices, high-score-first with stable ties."""
    if (isinstance(threshold, bool) or
            not isinstance(threshold, (int, float)) or
            not math.isfinite(threshold) or not 0 <= threshold <= 1):
        raise ValueError("threshold must be finite in [0, 1]")
    if not isinstance(boxes, (list, tuple)) or len(boxes) > 256:
        raise ValueError("boxes must be a sequence of at most 256 entries")
    values = []
    for box in boxes:
        if not isinstance(box, (list, tuple)) or len(box) != 5:
            raise ValueError("each box must contain five numeric values")
        if any(isinstance(v, bool) or not isinstance(v, (int, float))
               for v in box):
            raise ValueError("numeric box entries required")
        try:
            b = tuple(float(v) for v in box)
        except OverflowError as exc:
            raise ValueError("unrepresentable box coordinate") from exc
        if (any(not math.isfinite(v) for v in b) or
                any(abs(v) > 1_000_000 for v in b[:4]) or
                b[2] <= b[0] or b[3] <= b[1] or not 0 <= b[4] <= 1):
            raise ValueError("invalid coordinates or confidence score")
        values.append(b)

    def iou(a, b):
        w = max(0.0, min(a[2], b[2]) - max(a[0], b[0]))
        h = max(0.0, min(a[3], b[3]) - max(a[1], b[1]))
        intersection = w * h
        area_a = (a[2] - a[0]) * (a[3] - a[1])
        area_b = (b[2] - b[0]) * (b[3] - b[1])
        return intersection / (area_a + area_b - intersection)

    order = sorted(range(len(values)), key=lambda i: (-values[i][4], i))
    kept = []
    while order:
        best = order.pop(0)
        kept.append(best)
        order = [i for i in order if iou(values[best], values[i]) <= threshold]
    return kept
