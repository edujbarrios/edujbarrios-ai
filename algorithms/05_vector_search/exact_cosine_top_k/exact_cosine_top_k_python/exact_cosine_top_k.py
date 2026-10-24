"""Exact cosine-similarity Top-K over one dense vector collection."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real numbers required")
    try:
        number=float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable component") from exc
    if not math.isfinite(number):
        raise ValueError("finite real numbers required")
    return number

def _norm(vector: Sequence[float]) -> float:
    magnitude=0.0
    for value in vector:
        magnitude=math.hypot(magnitude,value)
        if not math.isfinite(magnitude):
            raise ValueError("norm overflow")
    if magnitude<=0.0:
        raise ValueError("zero vectors are not valid for cosine")
    return magnitude

def exact_cosine_top_k(query: Sequence[float],
                       candidates: Sequence[Sequence[float]],
                       k: int) -> list[int]:
    """Rank zero-based indices by decreasing cosine, then ascending index."""
    if not isinstance(query,(list,tuple)) or not query:
        raise ValueError("query must be a nonempty real vector")
    if not isinstance(candidates,(list,tuple)) or not candidates:
        raise ValueError("nonempty candidate matrix required")
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=len(candidates):
        raise ValueError("k must be an integer in 1..n")
    q=[_real(value) for value in query]
    query_norm=_norm(q)
    scored=[]
    for i,row in enumerate(candidates):
        if not isinstance(row,(list,tuple)) or len(row)!=len(q):
            raise ValueError("candidates must match query dimensionality")
        vector=[_real(value) for value in row]
        vector_norm=_norm(vector)
        score=0.0
        for a,b in zip(q,vector):
            term=(a/query_norm)*(b/vector_norm)
            score+=term
            if not math.isfinite(term) or not math.isfinite(score):
                raise ValueError("cosine accumulation overflow")
        scored.append((min(1.0,max(-1.0,score)),i))
    scored.sort(key=lambda pair:(-pair[0],pair[1]))
    return [index for _,index in scored[:k]]
