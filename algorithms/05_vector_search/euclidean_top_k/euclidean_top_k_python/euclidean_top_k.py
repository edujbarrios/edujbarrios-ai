"""Exact Euclidean Top-K, ordered by (distance, original row index)."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real vector coordinates required")
    try: number=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable coordinate") from exc
    if not math.isfinite(number):
        raise ValueError("nonfinite coordinate")
    return number

def euclidean_top_k(query: Sequence[float],
                    candidates: Sequence[Sequence[float]],k: int) -> list[int]:
    """Return zero-based indices of k nearest candidate vectors."""
    if not isinstance(query,(list,tuple)) or not query:
        raise ValueError("nonempty query required")
    if not isinstance(candidates,(list,tuple)) or not candidates:
        raise ValueError("nonempty candidates required")
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=len(candidates):
        raise ValueError("k must be an integer in 1..n")
    q=[_real(x) for x in query]
    rows=[]
    for row in candidates:
        if not isinstance(row,(list,tuple)) or len(row)!=len(q):
            raise ValueError("rectangular matrix matching query required")
        rows.append([_real(x) for x in row])
    scored=[]
    for i,row in enumerate(rows):
        distance=0.0
        for a,b in zip(q,row):
            delta=a-b
            if not math.isfinite(delta):
                raise ValueError("difference overflow")
            distance=math.hypot(distance,delta)
            if not math.isfinite(distance):
                raise ValueError("distance overflow")
        scored.append((distance,i))
    scored.sort()
    return [idx for _,idx in scored[:k]]
