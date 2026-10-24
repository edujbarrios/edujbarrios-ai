"""Tanh-approximated GELU activation and analytical derivative."""
import math
from typing import Sequence

ALPHA=0.7978845608028654
BETA=0.044715

def gelu(inputs: Sequence[float]) -> tuple[list[float],list[float]]:
    """Return elementwise (tanh-GELU values, derivatives) on finite reals."""
    if not isinstance(inputs,(list,tuple)) or not inputs:
        raise ValueError("nonempty finite input vector required")
    x=[]
    for value in inputs:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite real values required")
        try: real=float(value)
        except OverflowError as exc: raise ValueError("unrepresentable input") from exc
        if not math.isfinite(real):
            raise ValueError("nonfinite input")
        x.append(real)
    output=[]
    gradients=[]
    for v in x:
        square=v*v
        cubic=square*v
        u=ALPHA*(v+BETA*cubic)
        if not all(map(math.isfinite,(square,cubic,u))):
            raise ValueError("GELU polynomial overflow")
        t=math.tanh(u)
        gate=.5*(1.0+t)
        factor=ALPHA*(1.0+3.0*BETA*square)
        slope=.5*v*(1.0-t*t)*factor
        result=v*gate
        derivative=gate+slope
        if not all(map(math.isfinite,(factor,slope,result,derivative))):
            raise ValueError("GELU output overflow")
        output.append(result)
        gradients.append(derivative)
    return output,gradients
