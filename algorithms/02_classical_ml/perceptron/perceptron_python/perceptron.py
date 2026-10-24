"""Online binary perceptron learning with deterministic sample ordering."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real features required")
    try: number=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable feature") from exc
    if not math.isfinite(number):
        raise ValueError("finite real features required")
    return number

def perceptron_predict(weights: Sequence[float],
                       features: Sequence[float], bias: float) -> int:
    """Return class 1 only for strictly positive linear score."""
    if not weights or len(weights)!=len(features):
        raise ValueError("feature/weight dimensions must match and be nonempty")
    b=_real(bias)
    w=[_real(v) for v in weights]
    x=[_real(v) for v in features]
    score=0.0
    for a,v in zip(w,x):
        term=a*v
        score+=term
        if not math.isfinite(term) or not math.isfinite(score):
            raise ValueError("linear score overflow")
    score+=b
    if not math.isfinite(score):
        raise ValueError("linear score overflow")
    return int(score>0)

def perceptron(features: Sequence[Sequence[float]], labels: Sequence[int],
               learning_rate: float, epochs: int) -> tuple[list[float],float]:
    """Train and return (weights, bias) after a fixed number of online epochs."""
    if not isinstance(features,(list,tuple)) or not features or len(labels)!=len(features):
        raise ValueError("nonempty aligned training data required")
    if isinstance(epochs,bool) or not isinstance(epochs,int) or epochs<1:
        raise ValueError("epochs must be a positive integer")
    lr=_real(learning_rate)
    if lr<=0:
        raise ValueError("learning rate must be positive")
    if not isinstance(features[0],(list,tuple)) or not features[0]:
        raise ValueError("feature rows must be nonempty")
    d=len(features[0])
    rows=[]
    for row in features:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular numeric matrix required")
        rows.append([_real(x) for x in row])
    if any(isinstance(v,bool) or not isinstance(v,int) or v not in (0,1)
           for v in labels):
        raise ValueError("labels must be integer 0 or 1")
    weights=[0.0]*d
    bias=0.0
    for _ in range(epochs):
        for row,label in zip(rows,labels):
            pred=perceptron_predict(weights,row,bias)
            if pred==label:
                continue
            delta=lr*(1.0 if label==1 else -1.0)
            for j,value in enumerate(row):
                update=delta*value
                weights[j]+=update
                if not math.isfinite(update) or not math.isfinite(weights[j]):
                    raise ValueError("weight update overflow")
            bias+=delta
            if not math.isfinite(bias):
                raise ValueError("bias update overflow")
    return weights,bias
