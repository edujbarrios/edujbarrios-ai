"""Stable log-sum-exp reduction and its elementwise softmax gradient."""
import math
from typing import Sequence

def logsumexp(values:Sequence[float])->tuple[float,list[float]]:
    """Return (logsumexp, softmax derivative), rejecting finite overflow."""
    if not isinstance(values,(list,tuple)) or not 1<=len(values)<=4096:
        raise ValueError("finite input vector length must be 1..4096")
    x=[]
    for value in values:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("finite real logits required")
        try: number=float(value)
        except OverflowError as exc:raise ValueError("unrepresentable coordinate") from exc
        if not math.isfinite(number):raise ValueError("nonfinite coordinate")
        x.append(number)
    maximum=max(x)
    weights=[]
    total=0.0
    for number in x:
        shifted=number-maximum
        if not math.isfinite(shifted):raise ValueError("shifted logit overflow")
        weight=math.exp(shifted)
        total+=weight
        if not math.isfinite(weight) or not math.isfinite(total):
            raise ValueError("exponential weight overflow")
        weights.append(weight)
    if total<=0:raise ValueError("zero exponential normalizer")
    result=maximum+math.log(total)
    if not math.isfinite(result):raise ValueError("nonfinite logsumexp")
    gradient=[w/total for w in weights]
    if not all(map(math.isfinite,gradient)):
        raise ValueError("nonfinite softmax gradient")
    return result,gradient
