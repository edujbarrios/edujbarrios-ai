"""One stateful SGD-with-momentum update with gradient-accumulation velocity."""
import math
from typing import Sequence

def _real(value:float)->float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite numeric value required")
    try:v=float(value)
    except OverflowError as exc:raise ValueError("unrepresentable value") from exc
    if not math.isfinite(v):raise ValueError("nonfinite value")
    return v

def momentum_sgd(theta:Sequence[float],gradient:Sequence[float],
                 velocity:Sequence[float],learning_rate:float
                 )->tuple[list[float],list[float]]:
    """Return (updated_parameters, updated_velocity), using beta=0.9."""
    if (not isinstance(theta,(list,tuple)) or not theta or
        not isinstance(gradient,(list,tuple)) or
        not isinstance(velocity,(list,tuple)) or
        len(theta)!=len(gradient) or len(theta)!=len(velocity)):
        raise ValueError("equal nonempty vector lengths required")
    lr=_real(learning_rate)
    if lr<=0:raise ValueError("positive learning rate required")
    params=[_real(v) for v in theta]
    grads=[_real(v) for v in gradient]
    state=[_real(v) for v in velocity]
    next_params=[]
    next_velocity=[]
    for p,g,v in zip(params,grads,state):
        retained=.9*v
        new_velocity=retained+g
        delta=lr*new_velocity
        new_param=p-delta
        if not all(map(math.isfinite,(retained,new_velocity,delta,new_param))):
            raise ValueError("momentum update overflow")
        next_params.append(new_param)
        next_velocity.append(new_velocity)
    return next_params,next_velocity
