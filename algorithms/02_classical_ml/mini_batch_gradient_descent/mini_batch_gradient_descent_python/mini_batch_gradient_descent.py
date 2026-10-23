"""Deterministic mini-batch SGD for univariate linear regression."""
import math
from typing import Sequence

def _validate(values: Sequence[float]) -> list[float]:
    result=[]
    for value in values:
        if isinstance(value,bool) or not isinstance(value,(int,float)):
            raise ValueError("inputs must be finite real numbers")
        try:
            number=float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable input") from exc
        if not math.isfinite(number):
            raise ValueError("inputs must be finite")
        result.append(number)
    return result

def mini_batch_gradient_descent(
    x: Sequence[float], y: Sequence[float], learning_rate: float,
    epochs: int, batch_size: int,
) -> tuple[float,float]:
    """Return (weight, bias) trained by ordered mini-batch half-MSE gradients."""
    if not x or len(x)!=len(y):
        raise ValueError("nonempty paired inputs required")
    if isinstance(epochs,bool) or not isinstance(epochs,int) or epochs<=0:
        raise ValueError("epochs must be positive integer")
    if isinstance(batch_size,bool) or not isinstance(batch_size,int) or not 1<=batch_size<=len(x):
        raise ValueError("batch_size must be in 1..n")
    if isinstance(learning_rate,bool) or not isinstance(learning_rate,(int,float)):
        raise ValueError("learning_rate must be positive finite real number")
    try:
        lr=float(learning_rate)
    except OverflowError as exc:
        raise ValueError("unrepresentable learning rate") from exc
    if not math.isfinite(lr) or lr<=0:
        raise ValueError("learning_rate must be positive finite")
    xs,ys=_validate(x),_validate(y)
    weight=bias=0.0
    for _ in range(epochs):
        for start in range(0,len(xs),batch_size):
            end=min(start+batch_size,len(xs))
            gw=gb=0.0
            for i in range(start,end):
                prediction=weight*xs[i]+bias
                error=prediction-ys[i]
                contribution=error*xs[i]
                if not all(map(math.isfinite,(prediction,error,contribution))):
                    raise ValueError("prediction/gradient overflow")
                gw+=contribution
                gb+=error
                if not all(map(math.isfinite,(gw,gb))):
                    raise ValueError("gradient accumulation overflow")
            factor=lr/(end-start)
            dw,db=factor*gw,factor*gb
            if not all(map(math.isfinite,(factor,dw,db))):
                raise ValueError("parameter step overflow")
            weight-=dw
            bias-=db
            if not all(map(math.isfinite,(weight,bias))):
                raise ValueError("training diverged")
    return weight,bias
