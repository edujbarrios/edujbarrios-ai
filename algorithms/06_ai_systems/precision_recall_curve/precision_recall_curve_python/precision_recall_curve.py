"""Exact binary precision–recall curve at unique score thresholds."""
import math


def precision_recall_curve(scores: list[float], labels: list[int]) -> list[tuple[float, float, float]]:
    """Return (threshold, precision, recall), ordered by decreasing threshold."""
    if not isinstance(scores, (list, tuple)) or not isinstance(labels, (list, tuple)):
        raise ValueError("aligned score and label vectors required")
    if not scores or len(scores) != len(labels):
        raise ValueError("nonempty aligned vectors required")
    pairs = []
    positive_total = 0
    for score, label in zip(scores, labels):
        if isinstance(score, bool) or not isinstance(score, (int, float)):
            raise ValueError("scores must be real probabilities")
        try:
            number = float(score)
        except (OverflowError, ValueError) as exc:
            raise ValueError("unrepresentable score") from exc
        if not math.isfinite(number) or not 0.0 <= number <= 1.0:
            raise ValueError("scores must be finite probabilities in [0, 1]")
        if isinstance(label, bool) or not isinstance(label, int) or label not in (0, 1):
            raise ValueError("labels must be binary integers")
        pairs.append((number, label))
        positive_total += label
    if not positive_total:
        raise ValueError("at least one positive label is required")
    pairs.sort(key=lambda item: item[0], reverse=True)
    result = []
    true_positives = 0
    end = 0
    while end < len(pairs):
        start = end
        score = pairs[start][0]
        while end < len(pairs) and pairs[end][0] == score:
            true_positives += pairs[end][1]
            end += 1
        result.append((score, true_positives / end, true_positives / positive_total))
    return result
