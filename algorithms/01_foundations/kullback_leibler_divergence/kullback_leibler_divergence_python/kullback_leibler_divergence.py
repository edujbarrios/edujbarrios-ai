"""Base-2 directional KL divergence with an explicit infinite-support case."""
import math
from typing import Sequence

def _distribution(values:Sequence[float])->list[float]:
    if not isinstance(values,(list,tuple)) or not 1<=len(values)<=4096:
        raise ValueError("probability vector length must be 1..4096")
    result=[]
    total=0.0
    for value in values:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite real probabilities required")
        try: x=float(value)
        except OverflowError as exc:raise ValueError("unrepresentable probability") from exc
        if not math.isfinite(x) or not 0<=x<=1:
            raise ValueError("invalid probability")
        result.append(x)
        total+=x
        if not math.isfinite(total):
            raise ValueError("probability sum overflow")
    if abs(total-1)>1e-9:
        raise ValueError("probabilities must sum to one")
    return result

def kullback_leibler_divergence(p:Sequence[float],q:Sequence[float])->float:
    """Return KL_2(P||Q), or math.inf if P has mass outside Q's support."""
    a=_distribution(p)
    b=_distribution(q)
    if len(a)!=len(b):
        raise ValueError("aligned distribution lengths required")
    if any(x>0 and y==0 for x,y in zip(a,b)):
        return math.inf
    value=0.0
    for x,y in zip(a,b):
        if x:
            term=x*(math.log2(x)-math.log2(y))
            if not math.isfinite(term):
                raise ValueError("KL term overflow")
            value+=term
            if not math.isfinite(value):
                raise ValueError("KL sum overflow")
    if value< -1e-12:
        raise ValueError("negative divergence")
    return max(0.0,value)
