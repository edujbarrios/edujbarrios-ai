"""Mean reciprocal rank across first relevant result positions."""
from typing import Sequence

def mean_reciprocal_rank(ranks: Sequence[int]) -> float:
    """Mean of 1/rank, or zero for missed queries (rank 0)."""
    if not ranks:
        raise ValueError("at least one query rank is required")
    total = 0.0
    for rank in ranks:
        if isinstance(rank, bool) or not isinstance(rank, int) or rank < 0:
            raise ValueError("ranks must be nonnegative integers")
        if rank > 0:
            total += 1.0 / rank
    return total / len(ranks)
