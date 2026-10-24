"""Mean alpha-balanced binary focal loss with fixed gamma=2."""
import math
from typing import Sequence

def binary_focal_loss(probabilities:Sequence[float],labels:Sequence[int])->float:
    """Use alpha=.25, gamma=2, true-class probability floor of 1e-12."""
    if (not isinstance(probabilities,(list,tuple)) or
        not isinstance(labels,(list,tuple)) or
        not probabilities or len(probabilities)!=len(labels)):
        raise ValueError("nonempty equal-length probabilities and labels required")
    total=0.0
    for prob,label in zip(probabilities,labels):
        if isinstance(prob,bool) or not isinstance(prob,(int,float)):
            raise ValueError("finite real probabilities required")
        try:p=float(prob)
        except OverflowError as exc:raise ValueError("unrepresentable probability") from exc
        if (not math.isfinite(p) or not 0<=p<=1 or
            isinstance(label,bool) or not isinstance(label,int) or label not in (0,1)):
            raise ValueError("invalid probability or binary label")
        pt=p if label==1 else 1-p
        q=max(pt,1e-12)
        alpha=.25 if label==1 else .75
        loss=-alpha*(1-q)**2*math.log(q)
        total+=loss
        if not math.isfinite(loss) or not math.isfinite(total):
            raise ValueError("focal loss accumulation overflow")
    mean=total/len(probabilities)
    if not math.isfinite(mean) or mean<0:
        raise ValueError("invalid focal loss")
    return mean
