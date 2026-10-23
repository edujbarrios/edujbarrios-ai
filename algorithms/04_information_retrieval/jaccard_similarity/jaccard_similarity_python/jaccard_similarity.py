"""Jaccard intersection-over-union of signed 32-bit integer token sets."""
from typing import Sequence

MIN_ID = -(2 ** 31)
MAX_ID = 2 ** 31 - 1


def _unique_sorted(items: Sequence[int]) -> list[int]:
    converted = []
    for value in items:
        if isinstance(value, bool) or not isinstance(value, int) or not MIN_ID <= value <= MAX_ID:
            raise ValueError("token IDs must be signed 32-bit integers")
        converted.append(value)
    return sorted(set(converted))


def jaccard_similarity(left: Sequence[int], right: Sequence[int]) -> float:
    """Compute exact set intersection divided by union."""
    a, b = _unique_sorted(left), _unique_sorted(right)
    if not a and not b:
        return 1.0
    i = j = intersection = union = 0
    while i < len(a) or j < len(b):
        if j == len(b) or (i < len(a) and a[i] < b[j]):
            i += 1
        elif i == len(a) or b[j] < a[i]:
            j += 1
        else:
            intersection += 1
            i += 1
            j += 1
        union += 1
    return intersection / union
