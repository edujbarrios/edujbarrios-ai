"""Per-example layer normalization with affine gain and shift."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real components required")
    try:
        result=float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable component") from exc
    if not math.isfinite(result):
        raise ValueError("finite real components required")
    return result

def layer_normalization(input: Sequence[float], gamma: Sequence[float],
                        beta: Sequence[float], epsilon: float) -> list[float]:
    """Normalize one vector using population variance and per-feature affine."""
    n=len(input)
    if n==0 or len(gamma)!=n or len(beta)!=n:
        raise ValueError("nonempty equal-length feature vectors required")
    eps=_real(epsilon)
    if eps<=0.0:
        raise ValueError("epsilon must be positive")
    x=[_real(v) for v in input]
    gain=[_real(v) for v in gamma]
    shift=[_real(v) for v in beta]
    mean=0.0
    for i,value in enumerate(x,1):
        delta=value-mean
        if not math.isfinite(delta):
            raise ValueError("mean overflow")
        mean+=delta/i
        if not math.isfinite(mean):
            raise ValueError("mean overflow")
    squared_sum=0.0
    for value in x:
        delta=value-mean
        square=delta*delta
        if not math.isfinite(delta) or not math.isfinite(square):
            raise ValueError("variance overflow")
        squared_sum+=square
        if not math.isfinite(squared_sum):
            raise ValueError("variance accumulation overflow")
    variance=squared_sum/n
    divisor=math.sqrt(variance+eps)
    if not math.isfinite(divisor) or divisor<=0.0:
        raise ValueError("normalizer overflow")
    result=[]
    for value,g,b in zip(x,gain,shift):
        centered=value-mean
        normalized=centered/divisor
        weighted=g*normalized
        output=weighted+b
        if not all(map(math.isfinite,(centered,normalized,weighted,output))):
            raise ValueError("affine output overflow")
        result.append(output)
    return result
