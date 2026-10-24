"""One nonnegative Rocchio vector-query relevance-feedback update."""
import math
from typing import Sequence

def _real(x:float)->float:
    if isinstance(x,bool) or not isinstance(x,(int,float)):
        raise ValueError("finite nonnegative feature required")
    try:v=float(x)
    except OverflowError as exc:raise ValueError("unrepresentable feature") from exc
    if not math.isfinite(v) or not 0<=v<=1e6:
        raise ValueError("feature outside 0..1e6")
    return v

def rocchio_relevance_feedback(
    query:Sequence[float],
    relevant:Sequence[Sequence[float]],
    nonrelevant:Sequence[Sequence[float]]
)->list[float]:
    """Update q + 0.75 mean(relevant) - 0.15 mean(nonrelevant), clamp to 0."""
    if not isinstance(query,(list,tuple)) or not 1<=len(query)<=64:
        raise ValueError("query width must be 1..64")
    q=[_real(x) for x in query]
    d=len(q)
    def group(rows):
        if not isinstance(rows,(list,tuple)) or len(rows)>128:
            raise ValueError("feedback group must contain at most 128 rows")
        result=[]
        for row in rows:
            if not isinstance(row,(list,tuple)) or len(row)!=d:
                raise ValueError("feedback rows must match query dimension")
            result.append([_real(v) for v in row])
        return result
    pos=group(relevant)
    neg=group(nonrelevant)
    if not pos and not neg:
        raise ValueError("at least one feedback group must be nonempty")
    result=[]
    for j in range(d):
        positive=sum(row[j]/len(pos) for row in pos) if pos else 0.0
        negative=sum(row[j]/len(neg) for row in neg) if neg else 0.0
        value=q[j]+0.75*positive-0.15*negative
        if not math.isfinite(value):raise ValueError("Rocchio update overflow")
        result.append(max(0.0,value))
    return result
