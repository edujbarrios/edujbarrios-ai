"""Gini class impurity from bounded, nonnegative integer class counts."""


def gini_impurity(counts):
    """Return one minus the sum of squared class proportions."""
    if not isinstance(counts, (list, tuple)) or not 1 <= len(counts) <= 128:
        raise ValueError("class count vector of length 1..128 required")
    for count in counts:
        if isinstance(count, bool) or not isinstance(count, int) or not 0 <= count <= 1_000_000:
            raise ValueError("each class count must be an integer in 0..1000000")
    total = sum(counts)
    if total == 0:
        raise ValueError("at least one positive count is required")
    result = 1.0 - sum((count / total)**2 for count in counts)
    return max(0.0, min(1.0, result))
