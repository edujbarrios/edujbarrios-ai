"""One RMSProp optimizer step with explicit squared-gradient state."""
import math
from typing import Sequence

DECAY=0.9
EPSILON=1e-8

def _real(value:float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real values required")
    try: v=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable value") from exc
    if not math.isfinite(v): raise ValueError("nonfinite value")
    return v

def rmsprop_optimizer(parameters:Sequence[float],gradients:Sequence[float],
                      previous_average:Sequence[float],learning_rate:float
                      ) -> tuple[list[float],list[float]]:
    """Return (updated_parameters, updated_squared_average) with fixed decay."""
    if (not isinstance(parameters,(list,tuple)) or not parameters or
        not isinstance(gradients,(list,tuple)) or
        not isinstance(previous_average,(list,tuple)) or
        len(parameters)!=len(gradients) or len(parameters)!=len(previous_average)):
        raise ValueError("equal nonempty vector lengths required")
    eta=_real(learning_rate)
    if eta<=0: raise ValueError("learning_rate must be positive")
    theta=[_real(x) for x in parameters]
    g=[_real(x) for x in gradients]
    state=[_real(x) for x in previous_average]
    if any(x<0 for x in state):
        raise ValueError("negative squared-gradient average")
    next_theta=[]
    next_state=[]
    for p,grad,v in zip(theta,g,state):
        square=grad*grad
        average=DECAY*v+(1.0-DECAY)*square
        if not math.isfinite(square) or not math.isfinite(average) or average<0:
            raise ValueError("squared-gradient overflow")
        denominator=math.sqrt(average)+EPSILON
        step=eta*grad/denominator
        updated=p-step
        if not all(map(math.isfinite,(denominator,step,updated))):
            raise ValueError("RMSProp update overflow")
        next_theta.append(updated)
        next_state.append(average)
    return next_theta,next_state
