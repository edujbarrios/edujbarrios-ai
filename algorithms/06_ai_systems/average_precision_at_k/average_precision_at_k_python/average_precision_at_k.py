"""Average Precision at K with min(total_relevant, K) normalization."""
from typing import Sequence

def average_precision_at_k(relevance: Sequence[int],k: int,
                           total_relevant: int) -> float:
    """Return binary AP@K, with zero if no relevant corpus documents exist."""
    if not isinstance(relevance,(list,tuple)) or not relevance:
        raise ValueError("nonempty ranked relevance required")
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=len(relevance):
        raise ValueError("k must be an integer in 1..n")
    if (isinstance(total_relevant,bool) or not isinstance(total_relevant,int) or
        not 0<=total_relevant<=2**31-1):
        raise ValueError("invalid total relevant count")
    observed=0
    for flag in relevance:
        if isinstance(flag,bool) or not isinstance(flag,int) or flag not in (0,1):
            raise ValueError("relevance flags must be binary integers")
        observed+=flag
    if observed>total_relevant:
        raise ValueError("total relevant must cover observed relevance flags")
    if total_relevant==0:
        return 0.0
    hits=0
    weighted=0.0
    for i,flag in enumerate(relevance[:k],1):
        hits+=flag
        if flag:
            weighted+=hits/i
    return weighted/min(total_relevant,k)
