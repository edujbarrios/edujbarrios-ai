"""Numerically stable Swish (SiLU) and its analytical derivative."""
import math
from typing import Sequence

def swish(inputs: Sequence[float]) -> tuple[list[float],list[float]]:
    """Return (activations, derivatives) for a nonempty finite real vector."""
    if not isinstance(inputs,(list,tuple)) or not inputs:
        raise ValueError("nonempty real vector required")
    numbers=[]
    for x in inputs:
        if isinstance(x,bool) or not isinstance(x,(int,float)):
            raise ValueError("finite real values required")
        try: value=float(x)
        except OverflowError as exc: raise ValueError("unrepresentable value") from exc
        if not math.isfinite(value):
            raise ValueError("finite real values required")
        numbers.append(value)
    values=[]
    derivatives=[]
    for x in numbers:
        if x>=0:
            z=math.exp(-x)
            s=1.0/(1.0+z)
        else:
            z=math.exp(x)
            s=z/(1.0+z)
        value=x*s
        derivative=s+(x*s)*(1.0-s)
        if not math.isfinite(value) or not math.isfinite(derivative):
            raise ValueError("nonfinite Swish output")
        values.append(value)
        derivatives.append(derivative)
    return values,derivatives
