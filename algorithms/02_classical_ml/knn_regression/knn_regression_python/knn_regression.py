"""Unweighted k-nearest neighbors regression with deterministic distance ties."""
import math
from typing import Sequence

def _real(x:float)->float:
    if isinstance(x,bool) or not isinstance(x,(int,float)):
        raise ValueError("finite real numeric input required")
    try: number=float(x)
    except OverflowError as exc:raise ValueError("unrepresentable number") from exc
    if not math.isfinite(number):raise ValueError("nonfinite value")
    return number

def knn_regression(training:Sequence[Sequence[float]],
                   targets:Sequence[float],query:Sequence[float],k:int)->float:
    """Return the unweighted average target of the k Euclidean nearest rows."""
    if (not isinstance(training,(list,tuple)) or
        not 1<=len(training)<=4096 or
        not isinstance(training[0],(list,tuple)) or
        not 1<=len(training[0])<=64 or
        not isinstance(targets,(list,tuple)) or len(targets)!=len(training) or
        not isinstance(query,(list,tuple)) or len(query)!=len(training[0])):
        raise ValueError("aligned nonempty training, targets and query required")
    n=len(training)
    d=len(training[0])
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=n:
        raise ValueError("k must be an integer 1..n")
    q=[_real(x) for x in query]
    y=[_real(x) for x in targets]
    scored=[]
    for i,row in enumerate(training):
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular training matrix required")
        values=[_real(v) for v in row]
        distance=0.0
        for a,b in zip(values,q):
            delta=a-b
            if not math.isfinite(delta):raise ValueError("coordinate difference overflow")
            distance=math.hypot(distance,delta)
            if not math.isfinite(distance):raise ValueError("distance overflow")
        scored.append((distance,i))
    scored.sort()
    prediction=0.0
    for _,i in scored[:k]:
        contribution=y[i]/k
        if not math.isfinite(contribution):
            raise ValueError("target contribution overflow")
        prediction+=contribution
        if not math.isfinite(prediction):
            raise ValueError("prediction overflow")
    return prediction
