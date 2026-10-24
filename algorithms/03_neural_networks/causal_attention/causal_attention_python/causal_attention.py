"""Autoregressive single-head scaled self-attention over a whole sequence."""
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

def _matrix(data: Sequence[Sequence[float]], rows: int,
            columns: int) -> list[list[float]]:
    if not isinstance(data,(list,tuple)) or len(data)!=rows:
        raise ValueError("matrix row count mismatch")
    converted=[]
    for row in data:
        if not isinstance(row,(list,tuple)) or len(row)!=columns:
            raise ValueError("matrix must have matching nonempty row lengths")
        converted.append([_real(value) for value in row])
    return converted

def causal_attention(queries: Sequence[Sequence[float]],
                     keys: Sequence[Sequence[float]],
                     values: Sequence[Sequence[float]]) -> list[list[float]]:
    """Return T rows of causal attention, attending only to positions <= i."""
    if not isinstance(queries,(list,tuple)) or not queries or not isinstance(queries[0],(list,tuple)) or not queries[0]:
        raise ValueError("nonempty query sequence with positive dimension required")
    if not isinstance(values,(list,tuple)) or not values or not isinstance(values[0],(list,tuple)) or not values[0]:
        raise ValueError("nonempty value sequence with positive dimension required")
    t=len(queries);d=len(queries[0]);v=len(values[0])
    q=_matrix(queries,t,d)
    k=_matrix(keys,t,d)
    val=_matrix(values,t,v)
    scale=math.sqrt(d)
    output=[]
    for row in range(t):
        scores=[]
        for j in range(row+1):
            dot=0.0
            for a,b in zip(q[row],k[j]):
                term=a*b
                if not math.isfinite(term):
                    raise ValueError("attention product overflow")
                dot+=term
                if not math.isfinite(dot):
                    raise ValueError("attention sum overflow")
            score=dot/scale
            if not math.isfinite(score):
                raise ValueError("attention score overflow")
            scores.append(score)
        maximum=max(scores)
        exps=[]
        denominator=0.0
        for score in scores:
            shifted=score-maximum
            if not math.isfinite(shifted):
                raise ValueError("softmax shift overflow")
            item=math.exp(shifted)
            denominator+=item
            if not math.isfinite(denominator):
                raise ValueError("softmax denominator overflow")
            exps.append(item)
        result=[0.0]*v
        for j,exp_score in enumerate(exps):
            weight=exp_score/denominator
            for col in range(v):
                term=weight*val[j][col]
                if not math.isfinite(term):
                    raise ValueError("weighted value overflow")
                result[col]+=term
                if not math.isfinite(result[col]):
                    raise ValueError("output overflow")
        output.append(result)
    return output
