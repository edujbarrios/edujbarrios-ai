"""Inverse-distance-weighted k-nearest-neighbor regression."""
import math


def weighted_knn_regression(train, targets, query, k):
    """Predict from k nearest rows, using exact-match-only averaging at zero."""
    if type(k) is not int:
        raise ValueError("k must be an integer")
    if not isinstance(query, (list, tuple)) or not 1 <= len(query) <= 16:
        raise ValueError("query dimension must be 1..16")
    if (not isinstance(train, (list, tuple)) or not 1 <= len(train) <= 128
            or not isinstance(targets, (list, tuple))
            or len(targets) != len(train) or not 1 <= k <= len(train)):
        raise ValueError("invalid training shape, targets or k")

    def valid_real(value):
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("expected finite real observations")
        try:
            x = float(value)
        except OverflowError as exc:
            raise ValueError("value outside finite floating-point range") from exc
        if not math.isfinite(x) or abs(x) > 1000:
            raise ValueError("values must be finite in [-1000,1000]")
        return x

    q = [valid_real(v) for v in query]
    y = [valid_real(v) for v in targets]
    rows = []
    for row in train:
        if not isinstance(row, (list, tuple)) or len(row) != len(q):
            raise ValueError("training rows must match query dimensions")
        rows.append([valid_real(v) for v in row])
    distances = [
        (math.hypot(*(a-b for a, b in zip(row, q))), i)
        for i, row in enumerate(rows)
    ]
    distances.sort()
    selected = distances[:k]
    zeros = [y[i] for d, i in selected if d == 0.0]
    if zeros:
        return math.fsum(v/len(zeros) for v in zeros)
    minimum = selected[0][0]
    weights = [(minimum/d, i) for d, i in selected]
    numerator = math.fsum(weight*y[i] for weight, i in weights)
    denominator = math.fsum(weight for weight, _ in weights)
    prediction = numerator/denominator
    if not math.isfinite(prediction):
        raise ValueError("nonfinite prediction")
    return prediction
