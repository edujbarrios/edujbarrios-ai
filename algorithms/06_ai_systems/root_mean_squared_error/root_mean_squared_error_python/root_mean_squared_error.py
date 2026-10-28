"""Root mean squared prediction error with bounded finite inputs."""
import math


def root_mean_squared_error(target, prediction):
    """Compute sqrt(mean((prediction-target)**2)) in original units."""
    if not isinstance(target,(list,tuple)) or not isinstance(prediction,(list,tuple)):
        raise ValueError("paired numeric sequences required")
    n=len(target)
    if not 1<=n<=10000 or len(prediction)!=n:
        raise ValueError("equal lengths 1..10000 required")

    def checked(values):
        out=[]
        for value in values:
            if isinstance(value,bool) or not isinstance(value,(int,float)):
                raise ValueError("finite numeric values required")
            try:
                x=float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable observation") from exc
            if not math.isfinite(x) or abs(x)>1_000_000:
                raise ValueError("value outside allowed finite bounds")
            out.append(x)
        return out

    a,b=checked(target),checked(prediction)
    return math.sqrt(sum((p-t)**2 for t,p in zip(a,b))/n)
