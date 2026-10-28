"""Closed-form one-dimensional weighted linear regression with intercept."""
import math


def weighted_linear_regression_1d(x, y, weights):
    """Return (slope, intercept) minimizing weighted squared residuals."""
    if any(not isinstance(seq,(list,tuple)) for seq in (x,y,weights)):
        raise ValueError("three aligned numeric sequences required")
    n=len(x)
    if not 2<=n<=128 or len(y)!=n or len(weights)!=n:
        raise ValueError("equal lengths in 2..128 required")

    def checked(seq, nonnegative=False):
        output=[]
        for item in seq:
            if isinstance(item,bool) or not isinstance(item,(int,float)):
                raise ValueError("finite numeric observations required")
            try:
                val=float(item)
            except OverflowError as exc:
                raise ValueError("unrepresentable observation") from exc
            if not math.isfinite(val) or (
                not 0<=val<=1_000_000 if nonnegative else abs(val)>1_000_000
            ):
                raise ValueError("observation outside bounds")
            output.append(val)
        return output

    a,b,w=checked(x),checked(y),checked(weights,True)
    total=sum(w)
    if total<=0:
        raise ValueError("positive total sample weight required")
    mx=sum(wi*xi for wi,xi in zip(w,a))/total
    my=sum(wi*yi for wi,yi in zip(w,b))/total
    denominator=sum(wi*(xi-mx)**2 for wi,xi in zip(w,a))
    if denominator<=0 or not math.isfinite(denominator):
        raise ValueError("positive weighted feature variance required")
    numerator=sum(wi*(xi-mx)*(yi-my) for wi,xi,yi in zip(w,a,b))
    slope=numerator/denominator
    intercept=my-slope*mx
    if not math.isfinite(slope) or not math.isfinite(intercept):
        raise ValueError("nonfinite fit")
    return slope,intercept
