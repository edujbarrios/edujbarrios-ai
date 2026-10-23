"""Numerically stable scaled dot-product attention for a single query."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real numbers required")
    try:
        result=float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable feature") from exc
    if not math.isfinite(result):
        raise ValueError("finite real numbers required")
    return result

def scaled_dot_product_attention(
    query: Sequence[float], keys: Sequence[Sequence[float]],
    values: Sequence[Sequence[float]]
) -> list[float]:
    """Return the softmax-weighted sum of value vectors for one query."""
    if not isinstance(query,(list,tuple)) or not query:
        raise ValueError("query dimension must be positive")
    q=[_real(x) for x in query]
    if not isinstance(keys,(list,tuple)) or not isinstance(values,(list,tuple)) or not keys or len(keys)!=len(values):
        raise ValueError("keys and values require aligned nonempty rows")
    key_matrix=[]
    value_matrix=[]
    v=0
    for j,(key,value) in enumerate(zip(keys,values)):
        if not isinstance(key,(list,tuple)) or len(key)!=len(q):
            raise ValueError("key/query dimension mismatch")
        if not isinstance(value,(list,tuple)) or (j>0 and len(value)!=v) or not value:
            raise ValueError("value rows must be nonempty and rectangular")
        if j==0:
            v=len(value)
        key_matrix.append([_real(x) for x in key])
        value_matrix.append([_real(x) for x in value])
    scale=math.sqrt(len(q))
    scores=[]
    for key in key_matrix:
        dot=0.0
        for x,y in zip(q,key):
            product=x*y
            dot+=product
            if not math.isfinite(product) or not math.isfinite(dot):
                raise ValueError("dot overflow")
        score=dot/scale
        if not math.isfinite(score):
            raise ValueError("score overflow")
        scores.append(score)
    maximum=max(scores)
    exponents=[]
    denominator=0.0
    for score in scores:
        delta=score-maximum
        if not math.isfinite(delta):
            raise ValueError("softmax shift overflow")
        weight=math.exp(delta)
        exponents.append(weight)
        denominator+=weight
        if not math.isfinite(denominator):
            raise ValueError("softmax sum overflow")
    result=[0.0]*v
    for exponent,row in zip(exponents,value_matrix):
        weight=exponent/denominator
        for col,value in enumerate(row):
            term=weight*value
            result[col]+=term
            if not math.isfinite(term) or not math.isfinite(result[col]):
                raise ValueError("weighted value overflow")
    return result
