"""Deterministic top-p (nucleus) candidate token filtering."""
import math
from typing import Sequence

def top_p_nucleus_filtering(probabilities:Sequence[float],
                             threshold:float)->list[int]:
    """Return the shortest descending-probability prefix reaching threshold."""
    if not isinstance(probabilities,(list,tuple)) or not 1<=len(probabilities)<=4096:
        raise ValueError("probabilities must have length 1..4096")
    if isinstance(threshold,bool) or not isinstance(threshold,(int,float)):
        raise ValueError("threshold must be a probability")
    try:p=float(threshold)
    except OverflowError as exc:raise ValueError("unrepresentable threshold") from exc
    if not math.isfinite(p) or not 0<p<=1:
        raise ValueError("threshold must be in (0,1]")
    values=[]
    for x in probabilities:
        if isinstance(x,bool) or not isinstance(x,(int,float)):
            raise ValueError("finite real probabilities required")
        try:value=float(x)
        except OverflowError as exc:raise ValueError("unrepresentable probability") from exc
        if not math.isfinite(value) or not 0<=value<=1:
            raise ValueError("invalid probability")
        values.append(value)
    if abs(sum(values)-1)>1e-9:
        raise ValueError("distribution must sum to 1")
    ranked=sorted(range(len(values)),key=lambda i:(-values[i],i))
    chosen=[]
    cumulative=0.0
    for i in ranked:
        cumulative+=values[i]
        chosen.append(i)
        if cumulative>=p:break
    return chosen
