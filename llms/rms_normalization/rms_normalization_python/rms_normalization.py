"""Root mean square feature normalization without mean subtraction."""
import math


def rms_normalization(x, gain, epsilon):
    if not isinstance(x,(list,tuple)) or not isinstance(gain,(list,tuple)):
        raise ValueError("vectors required")
    if not 1 <= len(x) <= 1024 or len(gain)!=len(x):
        raise ValueError("dimension must be 1..1024 and gains must match")
    if isinstance(epsilon,bool) or not isinstance(epsilon,(int,float)):
        raise ValueError("numeric epsilon required")
    try:
        eps=float(epsilon)
    except OverflowError as exc:
        raise ValueError("unrepresentable epsilon") from exc
    if not math.isfinite(eps) or not 1e-8 <= eps <= 1:
        raise ValueError("invalid epsilon")
    def checked(items):
        result=[]
        for raw in items:
            if isinstance(raw,bool) or not isinstance(raw,(int,float)):
                raise ValueError("numeric vector required")
            try:
                v=float(raw)
            except OverflowError as exc:
                raise ValueError("unrepresentable vector value") from exc
            if not math.isfinite(v) or abs(v)>1000:
                raise ValueError("invalid feature or gain")
            result.append(v)
        return result
    features=checked(x)
    gains=checked(gain)
    inverse=1/math.sqrt(sum(v*v for v in features)/len(features)+eps)
    return [v*inverse*g for v,g in zip(features,gains)]
