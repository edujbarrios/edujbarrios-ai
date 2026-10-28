"""Mean squared log1p error on nonnegative regression pairs."""
import math


def mean_squared_log_error(target,prediction):
    """Return average (log1p(pred)-log1p(target))**2."""
    if not isinstance(target,(list,tuple)) or not isinstance(prediction,(list,tuple)):
        raise ValueError("paired numeric sequences required")
    n=len(target)
    if not 1<=n<=10000 or len(prediction)!=n:
        raise ValueError("equal lengths of 1..10000 required")

    def check(values):
        checked=[]
        for value in values:
            if isinstance(value,bool) or not isinstance(value,(int,float)):
                raise ValueError("numeric real inputs required")
            try:
                z=float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable value") from exc
            if not math.isfinite(z) or not 0<=z<=1_000_000:
                raise ValueError("input outside nonnegative finite bounds")
            checked.append(z)
        return checked

    a,b=check(target),check(prediction)
    return sum((math.log1p(p)-math.log1p(t))**2 for t,p in zip(a,b))/n
