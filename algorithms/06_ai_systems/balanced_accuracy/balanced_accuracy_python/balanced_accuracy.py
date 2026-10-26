"""Balanced accuracy for binary classification with both truth classes present."""

from typing import Sequence


def balanced_accuracy(truth: Sequence[int], predictions: Sequence[int]) -> float:
    """Return mean of true-positive and true-negative rates."""
    if not truth or len(truth) != len(predictions):
        raise ValueError("nonempty aligned vectors are required")
    positives = negatives = tp = tn = 0
    for y, p in zip(truth, predictions):
        if (isinstance(y, bool) or isinstance(p, bool)
                or not isinstance(y, int) or not isinstance(p, int)
                or y not in (0, 1) or p not in (0, 1)):
            raise ValueError("labels must be binary integers")
        if y:
            positives += 1
            tp += int(p == 1)
        else:
            negatives += 1
            tn += int(p == 0)
    if not positives or not negatives:
        raise ValueError("both truth classes must be present")
    return 0.5 * (tp / positives + tn / negatives)
