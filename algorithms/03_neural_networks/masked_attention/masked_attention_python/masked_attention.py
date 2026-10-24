"""One-query scaled dot-product attention with binary key masking."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value,(float,int)):
        raise ValueError("inputs must be finite real numbers")
    try:
        number=float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable input") from exc
    if not math.isfinite(number):
        raise ValueError("inputs must be finite")
    return number

def masked_attention(
    query: Sequence[float], keys: Sequence[Sequence[float]],
    values: Sequence[Sequence[float]], mask: Sequence[int]
) -> list[float]:
    """Return stable softmax-weighted values over only mask=1 rows."""
    if not isinstance(query,(list,tuple)) or not query:
        raise ValueError("positive query dimension required")
    q=[_real(v) for v in query]
    if not isinstance(keys,(list,tuple)) or not isinstance(values,(list,tuple)) or not keys or len(keys)!=len(values):
        raise ValueError("keys and values need aligned nonempty rows")
    if not isinstance(mask,(list,tuple)) or len(mask)!=len(keys) or any(isinstance(v,bool) or not isinstance(v,int) or v not in (0,1) for v in mask) or not any(mask):
        raise ValueError("mask must contain binary integers and at least one 1")
    key_rows=[]
    value_rows=[]
    value_dim=0
    for j,(key,value) in enumerate(zip(keys,values)):
        if not isinstance(key,(list,tuple)) or len(key)!=len(q):
            raise ValueError("key dimension mismatch")
        if not isinstance(value,(list,tuple)) or not value or (j>0 and len(value)!=value_dim):
            raise ValueError("values must be a rectangular nonempty matrix")
        if j==0:
            value_dim=len(value)
        key_rows.append([_real(x) for x in key])
        value_rows.append([_real(x) for x in value])

    active=[]
    maximum=-math.inf
    scale=math.sqrt(len(q))
    for j,key in enumerate(key_rows):
        if not mask[j]:
            continue
        dot=0.0
        for a,b in zip(q,key):
            product=a*b
            if not math.isfinite(product):
                raise ValueError("dot product overflow")
            dot+=product
            if not math.isfinite(dot):
                raise ValueError("dot sum overflow")
        score=dot/scale
        if not math.isfinite(score):
            raise ValueError("score overflow")
        maximum=max(maximum,score)
        active.append((j,score))

    exponents=[]
    denominator=0.0
    for j,score in active:
        shifted=score-maximum
        if not math.isfinite(shifted):
            raise ValueError("softmax shift overflow")
        exp_score=math.exp(shifted)
        denominator+=exp_score
        if not math.isfinite(denominator):
            raise ValueError("softmax denominator overflow")
        exponents.append((j,exp_score))

    output=[0.0]*value_dim
    for j,exp_score in exponents:
        probability=exp_score/denominator
        for col,value in enumerate(value_rows[j]):
            term=probability*value
            if not math.isfinite(term):
                raise ValueError("weighted value overflow")
            output[col]+=term
            if not math.isfinite(output[col]):
                raise ValueError("output overflow")
    return output
