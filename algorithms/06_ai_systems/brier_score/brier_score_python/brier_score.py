"""Binary Brier probability score, computed as mean squared error."""
import math
from typing import Sequence

def brier_score(probabilities:Sequence[float],labels:Sequence[int])->float:
    """Return average squared error for calibrated class-one probabilities."""
    if (not isinstance(probabilities,(list,tuple)) or
        not isinstance(labels,(list,tuple)) or
        not probabilities or len(probabilities)!=len(labels)):
        raise ValueError("aligned nonempty probability and label vectors required")
    total=0.0
    for p,label in zip(probabilities,labels):
        if isinstance(p,bool) or not isinstance(p,(int,float)):
            raise ValueError("probabilities must be finite real values")
        try: value=float(p)
        except OverflowError as exc: raise ValueError("unrepresentable probability") from exc
        if (not math.isfinite(value) or not 0<=value<=1 or
            isinstance(label,bool) or not isinstance(label,int) or label not in (0,1)):
            raise ValueError("probabilities must be in 0..1 and labels binary integers")
        error=value-label
        total+=error*error
        if not math.isfinite(total):
            raise ValueError("Brier accumulation overflow")
    score=total/len(probabilities)
    if not math.isfinite(score) or not 0<=score<=1+1e-12:
        raise ValueError("invalid Brier score")
    return min(1.0,score)
