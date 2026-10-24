"""Closed-form univariate ridge regression with an unpenalized intercept."""
import math
from typing import Sequence

def _as_values(data: Sequence[float]) -> list[float]:
    if not isinstance(data,(list,tuple)) or not 1<=len(data)<=4096:
        raise ValueError("input must be a nonempty bounded vector")
    values=[]
    for element in data:
        if isinstance(element,bool) or not isinstance(element,(int,float)):
            raise ValueError("finite real observations required")
        try: value=float(element)
        except OverflowError as exc: raise ValueError("unrepresentable observation") from exc
        if not math.isfinite(value) or abs(value)>1e6:
            raise ValueError("observation outside supported finite range")
        values.append(value)
    return values

def ridge_regression_1d(x:Sequence[float], y:Sequence[float],
                        penalty:float)->tuple[float,float]:
    """Return intercept, slope minimizing SSE + penalty*slope^2."""
    left=_as_values(x)
    right=_as_values(y)
    if len(left)!=len(right):raise ValueError("aligned vectors required")
    if isinstance(penalty,bool) or not isinstance(penalty,(int,float)):
        raise ValueError("finite nonnegative penalty required")
    try: lam=float(penalty)
    except OverflowError as exc:raise ValueError("unrepresentable penalty") from exc
    if not math.isfinite(lam) or not 0<=lam<=1e12:
        raise ValueError("penalty must be in [0,1e12]")
    n=len(left)
    mx=sum(a/n for a in left)
    my=sum(b/n for b in right)
    sxx=0.0
    sxy=0.0
    for a,b in zip(left,right):
        dx=a-mx
        dy=b-my
        sxx+=dx*dx
        sxy+=dx*dy
    denom=sxx+lam
    slope=0.0 if denom==0 else sxy/denom
    intercept=my-slope*mx
    if not all(map(math.isfinite,(mx,my,sxx,sxy,denom,slope,intercept))):
        raise ValueError("regression numeric overflow")
    return intercept,slope
