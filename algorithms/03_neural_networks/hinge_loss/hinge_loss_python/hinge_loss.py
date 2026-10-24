"""Mean binary hinge loss and deterministic score subgradients."""
import math
from typing import Sequence

def hinge_loss(scores:Sequence[float],labels:Sequence[int]
               )->tuple[float,list[float]]:
    """Return (mean loss, subgradients); zero subgradient at unit margin."""
    if (not isinstance(scores,(list,tuple)) or
        not isinstance(labels,(list,tuple)) or not scores or
        len(scores)!=len(labels)):
        raise ValueError("nonempty aligned score and label vectors required")
    n=len(scores)
    total=0.0
    gradients=[]
    for score,label in zip(scores,labels):
        if (isinstance(score,bool) or not isinstance(score,(int,float)) or
            isinstance(label,bool) or not isinstance(label,int) or
            label not in (-1,1)):
            raise ValueError("finite real scores and integer signed labels required")
        try:s=float(score)
        except OverflowError as exc:raise ValueError("unrepresentable score") from exc
        if not math.isfinite(s):raise ValueError("nonfinite score")
        margin=label*s
        deficit=1.0-margin
        if not math.isfinite(margin) or not math.isfinite(deficit):
            raise ValueError("margin overflow")
        total+=max(0.0,deficit)
        if not math.isfinite(total):raise ValueError("loss accumulation overflow")
        gradients.append(-label/n if margin<1.0 else 0.0)
    loss=total/n
    if not math.isfinite(loss):raise ValueError("loss overflow")
    return loss,gradients
