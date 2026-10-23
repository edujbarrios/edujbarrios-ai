"""Precision at K on an explicitly ordered binary-relevance sequence."""
from typing import Sequence

def precision_at_k(relevance: Sequence[int], k: int) -> float:
    """Return the relevant fraction in the first k items of a ranked list."""
    if isinstance(k, bool) or not isinstance(k, int) or k < 1:
        raise ValueError("k must be a positive integer")
    if not relevance or k > len(relevance):
        raise ValueError("ranking must be nonempty and contain at least k results")
    relevant = 0
    for i, label in enumerate(relevance):
        if isinstance(label, bool) or not isinstance(label, int) or label not in (0, 1):
            raise ValueError("all relevance labels must be integer 0 or 1")
        if i < k:
            relevant += label
    return relevant / k
