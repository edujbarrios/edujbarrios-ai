"""Tie-aware non-interpolated average precision for binary classifiers."""

import math


def binary_average_precision(y_true, y_scores):
    """Return classification AP over all unique descending score thresholds."""
    if (not isinstance(y_true, (list, tuple)) or
            not isinstance(y_scores, (list, tuple)) or
            not 1 <= len(y_true) <= 10_000 or
            len(y_scores) != len(y_true)):
        raise ValueError("aligned label and score arrays of length 1..10000 required")

    pairs = []
    positive_count = 0
    for label, score in zip(y_true, y_scores):
        if type(label) is not int or label not in (0, 1):
            raise ValueError("labels must be integer zeros or ones")
        if isinstance(score, bool) or not isinstance(score, (int, float)):
            raise ValueError("scores must be bounded finite real numbers")
        try:
            value = float(score)
        except (OverflowError, ValueError):
            raise ValueError("scores must be bounded finite real numbers") from None
        if not math.isfinite(value) or not -1_000_000 <= value <= 1_000_000:
            raise ValueError("scores must be bounded finite real numbers")
        pairs.append((value, label))
        positive_count += label

    if not positive_count:
        return 0.0

    pairs.sort(key=lambda item: -item[0])
    true_positives = 0
    seen = 0
    average_precision = 0.0
    previous_recall = 0.0
    index = 0
    while index < len(pairs):
        score = pairs[index][0]
        while index < len(pairs) and pairs[index][0] == score:
            true_positives += pairs[index][1]
            index += 1
            seen += 1
        precision = true_positives / seen
        recall = true_positives / positive_count
        average_precision += (recall - previous_recall) * precision
        previous_recall = recall
    return min(1.0, max(0.0, average_precision))
