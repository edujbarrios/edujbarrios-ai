"""Deterministic reciprocal rank fusion of two integer-ID result lists."""

from typing import Sequence

MAX_ID = 2_147_483_647


def reciprocal_rank_fusion(a: Sequence[int], b: Sequence[int]) -> list[int]:
    """Return fused unique IDs, ordered by decreasing RRF score then ID."""
    if not isinstance(a, (list, tuple)) or not isinstance(b, (list, tuple)):
        raise ValueError("rankings must be sequences")
    if not a and not b:
        raise ValueError("at least one ranking must be nonempty")
    scores: dict[int, float] = {}
    for ranking in (a, b):
        seen: set[int] = set()
        for rank, item in enumerate(ranking, start=1):
            if isinstance(item, bool) or not isinstance(item, int) or not 0 <= item <= MAX_ID:
                raise ValueError("IDs must be nonnegative 32-bit signed integers")
            if item in seen:
                raise ValueError("duplicate ID within one ranking")
            seen.add(item)
            scores[item] = scores.get(item, 0.0) + 1.0 / (60.0 + rank)
    return sorted(scores, key=lambda item: (-scores[item], item))
