"""Shannon entropy of a finite categorical distribution, in bits."""
import math
from typing import Sequence

def shannon_entropy(probabilities:Sequence[float])->float:
    """Compute -sum(p*log2(p)); zero-valued terms contribute zero."""
    if (not isinstance(probabilities,(list,tuple)) or
        not 1<=len(probabilities)<=4096):
        raise ValueError("probability vector length must be 1..4096")
    values=[]
    total=0.0
    for p in probabilities:
        if isinstance(p,bool) or not isinstance(p,(int,float)):
            raise ValueError("finite real probabilities required")
        try: value=float(p)
        except OverflowError as exc: raise ValueError("unrepresentable probability") from exc
        if not math.isfinite(value) or not 0<=value<=1:
            raise ValueError("probabilities must be finite in [0,1]")
        values.append(value)
        total+=value
        if not math.isfinite(total):
            raise ValueError("probability sum overflow")
    if abs(total-1)>1e-9:
        raise ValueError("probabilities must sum to one")
    result=0.0
    for value in values:
        if value:
            result-=value*math.log2(value)
        if not math.isfinite(result):
            raise ValueError("entropy accumulation overflow")
    if result< -1e-12:
        raise ValueError("negative information")
    return max(0.0,result)
