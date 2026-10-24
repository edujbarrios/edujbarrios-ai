"""Exact maximum-inner-product top-k over finite dense vectors."""
import math
from typing import Sequence

def _real(value:float)->float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real coordinate required")
    try: x=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable coordinate") from exc
    if not math.isfinite(x): raise ValueError("nonfinite coordinate")
    return x

def maximum_inner_product_top_k(
    query:Sequence[float],candidates:Sequence[Sequence[float]],k:int
)->list[int]:
    """Return zero-based row indices by descending dot product, then index."""
    if not isinstance(query,(list,tuple)) or not query:
        raise ValueError("nonempty query required")
    if not isinstance(candidates,(list,tuple)) or not candidates:
        raise ValueError("nonempty candidates required")
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=len(candidates):
        raise ValueError("k must be integer 1..n")
    q=[_real(v) for v in query]
    rows=[]
    for row in candidates:
        if not isinstance(row,(list,tuple)) or len(row)!=len(q):
            raise ValueError("rectangular matrix matching query required")
        rows.append([_real(v) for v in row])
    scores=[]
    for i,row in enumerate(rows):
        score=0.0
        for a,b in zip(q,row):
            product=a*b
            if not math.isfinite(product):raise ValueError("dot product overflow")
            score+=product
            if not math.isfinite(score):raise ValueError("dot sum overflow")
        scores.append((-score,i))
    scores.sort()
    return [index for _,index in scores[:k]]
