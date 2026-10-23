"""Recall at k on an ordered binary relevance sequence."""
from typing import Sequence

def recall_at_k(relevance: Sequence[int], k: int, total_relevant: int) -> float:
    """Fraction of all relevant documents recovered in the top k results."""
    if isinstance(k, bool) or not isinstance(k, int) or k < 1:
        raise ValueError("k must be a positive integer")
    if isinstance(total_relevant, bool) or not isinstance(total_relevant, int) or total_relevant < 1:
        raise ValueError("total_relevant must be a positive integer")
    if not relevance or k > len(relevance):
        raise ValueError("ranking must be nonempty and contain at least k results")
    observed = top_relevant = 0
    for index, label in enumerate(relevance):
        if isinstance(label, bool) or not isinstance(label, int) or label not in (0, 1):
            raise ValueError("relevance labels must be integer 0 or 1")
        observed += label
        if index < k:
            top_relevant += label
    if observed > total_relevant:
        raise ValueError("more observed relevant results than total_relevant")
    return top_relevant / total_relevant
