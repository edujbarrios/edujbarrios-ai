"""Graded-relevance NDCG@K with exponential gain and binary log discount."""
import math
from typing import Sequence


def ndcg_at_k(grades: Sequence[int], k: int) -> float:
    """Compute the normalized discounted gain of the first k results."""
    if isinstance(k, bool) or not isinstance(k, int) or k <= 0:
        raise ValueError("k must be a positive integer")
    if not grades or k > len(grades):
        raise ValueError("ranking must be nonempty and contain at least k grades")
    for grade in grades:
        if isinstance(grade, bool) or not isinstance(grade, int) or not 0 <= grade <= 20:
            raise ValueError("grades must be integer values in 0..20")

    def dcg(values: Sequence[int]) -> float:
        return sum(((2 ** grade) - 1) / math.log2(index + 2)
                   for index, grade in enumerate(values[:k]))

    current = dcg(grades)
    ideal = dcg(sorted(grades, reverse=True))
    return min(1.0, max(0.0, current / ideal)) if ideal else 0.0
