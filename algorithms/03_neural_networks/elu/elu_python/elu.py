"""Exponential linear unit with alpha fixed at one."""
import math


def elu(values):
    """Return x when nonnegative, otherwise expm1(x), elementwise."""
    if not isinstance(values,(list,tuple)) or not 1<=len(values)<=10000:
        raise ValueError("vector length must be in 1..10000")
    checked=[]
    for value in values:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite numeric input required")
        try:
            x=float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable input") from exc
        if not math.isfinite(x) or abs(x)>1_000_000:
            raise ValueError("value outside finite bounds")
        checked.append(x)
    return [x if x>=0 else math.expm1(x) for x in checked]
