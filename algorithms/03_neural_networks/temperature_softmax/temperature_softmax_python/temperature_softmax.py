"""Numerically stable temperature-scaled softmax on finite logits."""
import math
from typing import Sequence

def temperature_softmax(logits:Sequence[float],temperature:float)->list[float]:
    """Return the categorical probabilities using a strictly positive temperature."""
    if not isinstance(logits,(list,tuple)) or not 1<=len(logits)<=4096:
        raise ValueError("nonempty logit vector of length 1..4096 required")
    if isinstance(temperature,bool) or not isinstance(temperature,(int,float)):
        raise ValueError("finite positive temperature required")
    try: t=float(temperature)
    except OverflowError as exc:raise ValueError("unrepresentable temperature") from exc
    if not math.isfinite(t) or t<=0:raise ValueError("finite positive temperature required")
    values=[]
    for logit in logits:
        if isinstance(logit,bool) or not isinstance(logit,(int,float)):
            raise ValueError("finite real logits required")
        try:value=float(logit)
        except OverflowError as exc:raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(value):raise ValueError("nonfinite logit")
        values.append(value)
    maximum=max(values)
    weights=[]
    for value in values:
        shifted=value-maximum
        scaled=shifted/t
        if not math.isfinite(shifted) or not math.isfinite(scaled):
            raise ValueError("softmax scaling overflow")
        weights.append(math.exp(scaled))
    total=sum(weights)
    if not math.isfinite(total) or total<=0:
        raise ValueError("invalid exponential normalization")
    probabilities=[w/total for w in weights]
    if not all(map(math.isfinite,probabilities)):
        raise ValueError("nonfinite softmax probability")
    return probabilities
