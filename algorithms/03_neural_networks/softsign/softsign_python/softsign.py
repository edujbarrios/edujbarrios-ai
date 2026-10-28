"""Softsign forward activation for bounded finite real-valued vectors."""
import math


def softsign(values):
    """Return elementwise x/(1+abs(x)), rejecting malformed inputs."""
    if not isinstance(values,(list,tuple)) or not 1<=len(values)<=10000:
        raise ValueError("vector length 1..10000 required")
    checked=[]
    for value in values:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite real inputs required")
        try:
            x=float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable input") from exc
        if not math.isfinite(x) or abs(x)>1_000_000:
            raise ValueError("input outside finite bounds")
        checked.append(x)
    return [x/(1+abs(x)) for x in checked]
