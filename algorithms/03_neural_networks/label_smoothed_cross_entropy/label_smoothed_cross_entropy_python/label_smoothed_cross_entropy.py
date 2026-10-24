"""Label-smoothed softmax cross-entropy and analytic logit gradient."""
import math
from typing import Sequence

def label_smoothed_cross_entropy(logits:Sequence[float],
                                 target:int,epsilon:float
                                 )->tuple[float,list[float]]:
    """Return single-sample cross-entropy and gradients of each logit."""
    if not isinstance(logits,(list,tuple)) or not 1<=len(logits)<=4096:
        raise ValueError("logits length must be 1..4096")
    n=len(logits)
    if isinstance(target,bool) or not isinstance(target,int) or not 0<=target<n:
        raise ValueError("target must be a zero-based class index")
    if isinstance(epsilon,bool) or not isinstance(epsilon,(int,float)):
        raise ValueError("smoothing must be a finite real")
    try: smoothing=float(epsilon)
    except OverflowError as exc:raise ValueError("invalid smoothing") from exc
    if not math.isfinite(smoothing) or not 0<=smoothing<=1:
        raise ValueError("smoothing must lie in [0,1]")
    z=[]
    for x in logits:
        if isinstance(x,bool) or not isinstance(x,(int,float)):
            raise ValueError("logits must be finite reals")
        try:v=float(x)
        except OverflowError as exc:raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(v):raise ValueError("nonfinite logit")
        z.append(v)
    maximum=max(z)
    shifted=[x-maximum for x in z]
    if not all(map(math.isfinite,shifted)):
        raise ValueError("logit difference overflow")
    weights=[math.exp(x) for x in shifted]
    normalizer=sum(weights)
    if not math.isfinite(normalizer) or normalizer<=0:
        raise ValueError("invalid softmax normalizer")
    logz=math.log(normalizer)
    loss=0.0
    gradients=[]
    for i,x in enumerate(shifted):
        q=smoothing/n+(1-smoothing if i==target else 0.0)
        term=q*(logz-x)
        loss+=term
        grad=weights[i]/normalizer-q
        if not all(map(math.isfinite,(term,loss,grad))):
            raise ValueError("loss or gradient overflow")
        gradients.append(grad)
    if loss< -1e-12:raise ValueError("invalid negative loss")
    return max(0.0,loss),gradients
