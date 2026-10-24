"""Symmetric Jensen–Shannon divergence for discrete probabilities (base 2)."""
import math
from typing import Sequence

def jensen_shannon_divergence(p:Sequence[float],q:Sequence[float])->float:
    """Calculate JSD base 2, treating zero probability terms as zero."""
    if (not isinstance(p,(list,tuple)) or not isinstance(q,(list,tuple)) or
        not 1<=len(p)<=4096 or len(p)!=len(q)):
        raise ValueError("equal probability vectors of length 1..4096 required")
    converted=[]
    for values in (p,q):
        row=[]
        for value in values:
            if isinstance(value,bool) or not isinstance(value,(int,float)):
                raise ValueError("real probabilities required")
            try: x=float(value)
            except OverflowError as exc:raise ValueError("unrepresentable probability") from exc
            if not math.isfinite(x) or not 0<=x<=1:
                raise ValueError("invalid probability")
            row.append(x)
        if abs(sum(row)-1)>1e-9:
            raise ValueError("probability vector must sum to one")
        converted.append(row)
    left,right=converted
    result=0.0
    for a,b in zip(left,right):
        mixture=.5*a+.5*b
        if a:result+=.5*a*math.log2(a/mixture)
        if b:result+=.5*b*math.log2(b/mixture)
    if not math.isfinite(result) or not -1e-12<=result<=1+1e-12:
        raise ValueError("invalid JSD")
    return max(0.0,min(1.0,result))
