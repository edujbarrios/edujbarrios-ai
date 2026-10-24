"""One bias-corrected AdamW optimizer update with decoupled weight decay."""
import math
from typing import Sequence

def _real(value:float)->float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real number required")
    try: v=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable number") from exc
    if not math.isfinite(v): raise ValueError("nonfinite number")
    return v

def adamw_optimizer(theta:Sequence[float],gradient:Sequence[float],
                    m:Sequence[float],v:Sequence[float],step:int,
                    learning_rate:float)->tuple[list[float],list[float],list[float]]:
    """Return updated (parameters, first moment, second moment)."""
    if (not isinstance(theta,(list,tuple)) or not theta or
        any(not isinstance(x,(list,tuple)) or len(x)!=len(theta)
            for x in (gradient,m,v))):
        raise ValueError("four equal nonempty vectors required")
    if isinstance(step,bool) or not isinstance(step,int) or not 1<=step<=1_000_000:
        raise ValueError("step must be integer 1..1,000,000")
    eta=_real(learning_rate)
    if eta<=0: raise ValueError("positive learning rate required")
    params=[_real(x) for x in theta]
    gradients=[_real(x) for x in gradient]
    first=[_real(x) for x in m]
    second=[_real(x) for x in v]
    if any(x<0 for x in second): raise ValueError("negative second moment")
    correction1=1-.9**step
    correction2=1-.999**step
    updated=[]
    next_m=[]
    next_v=[]
    for p,g,old_m,old_v in zip(params,gradients,first,second):
        square=g*g
        new_m=.9*old_m+.1*g
        new_v=.999*old_v+.001*square
        if not all(map(math.isfinite,(square,new_m,new_v))) or new_v<0:
            raise ValueError("moment overflow")
        corrected_m=new_m/correction1
        corrected_v=new_v/correction2
        denominator=math.sqrt(corrected_v)+1e-8
        adaptive=corrected_m/denominator
        decay=.01*p
        change=eta*(adaptive+decay)
        new_p=p-change
        if not all(map(math.isfinite,(corrected_m,corrected_v,denominator,adaptive,decay,change,new_p))):
            raise ValueError("AdamW update overflow")
        updated.append(new_p)
        next_m.append(new_m)
        next_v.append(new_v)
    return updated,next_m,next_v
