"""Bias-corrected Adam optimization, one explicit parameter step."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value, (float,int)):
        raise ValueError("finite real components required")
    try:
        result = float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable value") from exc
    if not math.isfinite(result):
        raise ValueError("finite real components required")
    return result

def adam_optimizer(parameters: Sequence[float], gradients: Sequence[float],
                   m: Sequence[float], v: Sequence[float],
                   learning_rate: float, step: int
                   ) -> tuple[list[float],list[float],list[float]]:
    """Return (new_parameters, new_m, new_v) with one-based step index."""
    n=len(parameters)
    if n==0 or len(gradients)!=n or len(m)!=n or len(v)!=n:
        raise ValueError("nonempty equal-length arrays required")
    if isinstance(step,bool) or not isinstance(step,int) or step<=0:
        raise ValueError("step must be a positive integer")
    lr=_real(learning_rate)
    if lr<=0:
        raise ValueError("learning rate must be strictly positive")
    p=[_real(x) for x in parameters]
    g=[_real(x) for x in gradients]
    old_m=[_real(x) for x in m]
    old_v=[_real(x) for x in v]
    if any(x<0 for x in old_v):
        raise ValueError("second moments must be nonnegative")
    b1,b2,eps=0.9,0.999,1e-8
    c1=-math.expm1(step*math.log(b1))
    c2=-math.expm1(step*math.log(b2))
    if c1<=0 or c2<=0:
        raise ValueError("invalid moment correction")
    new_p=[];new_m=[];new_v=[]
    for theta,gradient,first,second in zip(p,g,old_m,old_v):
        square=gradient*gradient
        next_m=b1*first+(1-b1)*gradient
        next_v=b2*second+(1-b2)*square
        if not all(map(math.isfinite,(square,next_m,next_v))) or next_v<0:
            raise ValueError("moment overflow")
        mhat=next_m/c1
        vhat=next_v/c2
        if not all(map(math.isfinite,(mhat,vhat))) or vhat<0:
            raise ValueError("bias correction overflow")
        delta=lr*(mhat/(math.sqrt(vhat)+eps))
        next_p=theta-delta
        if not math.isfinite(delta) or not math.isfinite(next_p):
            raise ValueError("parameter update overflow")
        new_p.append(next_p);new_m.append(next_m);new_v.append(next_v)
    return new_p,new_m,new_v
