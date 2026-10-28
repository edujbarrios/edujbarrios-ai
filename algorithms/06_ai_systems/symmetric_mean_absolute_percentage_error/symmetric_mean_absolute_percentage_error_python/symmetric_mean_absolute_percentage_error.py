"""Symmetric relative prediction error with zero/zero defined as zero."""
import math


def symmetric_mean_absolute_percentage_error(target, prediction):
    """Return mean symmetric error ratio in [0, 2], not percentage units."""
    if not isinstance(target,(list,tuple)) or not isinstance(prediction,(list,tuple)):
        raise ValueError("paired numerical sequences required")
    n=len(target)
    if not 1<=n<=10000 or len(prediction)!=n:
        raise ValueError("equal lengths in 1..10000 required")

    def checked(values):
        result=[]
        for item in values:
            if isinstance(item,bool) or not isinstance(item,(int,float)):
                raise ValueError("real finite values required")
            try:
                value=float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable input") from exc
            if not math.isfinite(value) or abs(value)>1_000_000:
                raise ValueError("input outside bounds")
            result.append(value)
        return result

    a,b=checked(target),checked(prediction)
    total=0.0
    for t,p in zip(a,b):
        denom=abs(t)+abs(p)
        if denom>0.0:
            total+=2.0*abs(p-t)/denom
    return max(0.0,min(2.0,total/n))
