"""Train the smallest two-layer ReLU-to-sigmoid classifier from scratch."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value, (int,float)):
        raise ValueError("finite real values required")
    try:
        number=float(value)
    except OverflowError as exc:
        raise ValueError("unrepresentable component") from exc
    if not math.isfinite(number):
        raise ValueError("finite real values required")
    return number

def _sigmoid(logit: float) -> float:
    if logit >= 0.0:
        e=math.exp(-logit)
        return 1.0/(1.0+e)
    e=math.exp(logit)
    return e/(1.0+e)

def two_layer_predict(params: Sequence[float], feature: float) -> float:
    """Predict class-one probability from (w1,b1,w2,b2)."""
    if len(params)!=4:
        raise ValueError("exactly four parameters required")
    w1,b1,w2,b2=(_real(p) for p in params)
    x=_real(feature)
    pre=w1*x+b1
    if not math.isfinite(pre):
        raise ValueError("hidden preactivation overflow")
    logit=w2*max(0.0,pre)+b2
    if not math.isfinite(logit):
        raise ValueError("output logit overflow")
    return _sigmoid(logit)

def two_layer_binary_classifier(
    features: Sequence[float], labels: Sequence[float],
    learning_rate: float, epochs: int
) -> tuple[float,float,float,float]:
    """Return fitted (w1,b1,w2,b2) after exact full-batch epochs."""
    if not features or len(features)!=len(labels):
        raise ValueError("nonempty equal-length features and labels required")
    if isinstance(epochs,bool) or not isinstance(epochs,int) or epochs<1:
        raise ValueError("epochs must be a positive integer")
    lr=_real(learning_rate)
    if lr<=0.0:
        raise ValueError("learning_rate must be positive")
    xs=[_real(v) for v in features]
    ys=[_real(v) for v in labels]
    if any(v not in (0.0,1.0) for v in ys):
        raise ValueError("labels must be binary")
    params=[1.0,0.5,1.0,0.0]
    for _ in range(epochs):
        gradient=[0.0,0.0,0.0,0.0]
        for x,y in zip(xs,ys):
            pre=params[0]*x+params[1]
            if not math.isfinite(pre):
                raise ValueError("hidden preactivation overflow")
            h=max(0.0,pre)
            logit=params[2]*h+params[3]
            if not math.isfinite(logit):
                raise ValueError("output logit overflow")
            error=_sigmoid(logit)-y
            hidden_delta=error*params[2] if pre>0.0 else 0.0
            terms=(hidden_delta*x,hidden_delta,error*h,error)
            for j,term in enumerate(terms):
                if not math.isfinite(term):
                    raise ValueError("gradient product overflow")
                gradient[j]+=term
                if not math.isfinite(gradient[j]):
                    raise ValueError("gradient accumulation overflow")
        factor=lr/len(xs)
        for j in range(4):
            update=factor*gradient[j]
            if not math.isfinite(update):
                raise ValueError("parameter update overflow")
            params[j]-=update
            if not math.isfinite(params[j]):
                raise ValueError("training diverged")
    return tuple(params)
