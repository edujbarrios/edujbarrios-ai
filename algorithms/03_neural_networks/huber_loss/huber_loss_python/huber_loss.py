"""Mean robust Huber regression loss and prediction gradients."""
import math
from typing import Sequence

def _real(value:float)->float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real numeric input required")
    try:result=float(value)
    except OverflowError as exc:raise ValueError("unrepresentable number") from exc
    if not math.isfinite(result):raise ValueError("nonfinite input")
    return result

def huber_loss(predictions:Sequence[float],targets:Sequence[float],
               delta:float)->tuple[float,list[float]]:
    """Return (batch-mean Huber loss, gradient wrt each prediction)."""
    if (not isinstance(predictions,(list,tuple)) or
        not isinstance(targets,(list,tuple)) or not predictions or
        len(predictions)!=len(targets)):
        raise ValueError("aligned nonempty vectors required")
    threshold=_real(delta)
    if threshold<=0:raise ValueError("delta must be positive")
    p=[_real(x) for x in predictions]
    y=[_real(x) for x in targets]
    total=0.0
    gradients=[]
    n=len(p)
    for prediction,target in zip(p,y):
        error=prediction-target
        if not math.isfinite(error):raise ValueError("residual overflow")
        magnitude=abs(error)
        if magnitude<=threshold:
            sample=.5*error*error
            gradient=error/n
        else:
            sample=threshold*(magnitude-.5*threshold)
            gradient=(threshold if error>0 else -threshold)/n
        if not math.isfinite(sample) or not math.isfinite(gradient):
            raise ValueError("Huber arithmetic overflow")
        total+=sample
        if not math.isfinite(total):raise ValueError("Huber sum overflow")
        gradients.append(gradient)
    result=total/n
    if not math.isfinite(result) or result<0:raise ValueError("invalid Huber result")
    return result,gradients
