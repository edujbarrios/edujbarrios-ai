"""Exact top-k ranking of 30-bit LSH signatures by Hamming distance."""
from typing import Sequence

MAX_SIGNATURE = (1 << 30) - 1

def _check_signature(value: int) -> int:
    if isinstance(value, bool) or not isinstance(value, int) or not 0 <= value <= MAX_SIGNATURE:
        raise ValueError("signatures must be integers in 0..2^30-1")
    return value

def hamming_top_k(query: int, candidates: Sequence[int], k: int) -> list[int]:
    """Return zero-based candidate indices by ascending XOR popcount and index."""
    query = _check_signature(query)
    if isinstance(k, bool) or not isinstance(k, int) or k < 1 or not candidates or k > len(candidates):
        raise ValueError("k must be an integer in 1..number of candidates")
    signatures = [_check_signature(v) for v in candidates]
    return sorted(range(len(signatures)),
                  key=lambda i: ((query ^ signatures[i]).bit_count(), i))[:k]
