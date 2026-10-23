"""Backward pass of one affine layer followed by ReLU."""
import math
from typing import Sequence

def _finite(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(float,int)):
        raise ValueError("finite real components required")
    try: result=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable component") from exc
    if not math.isfinite(result):
        raise ValueError("finite real components required")
    return result

def dense_relu_backprop(x: Sequence[float],
                        weights: Sequence[Sequence[float]],
                        bias: Sequence[float],
                        upstream: Sequence[float]
                        ) -> tuple[list[float],list[list[float]],list[float]]:
    """Return dL/dx, dL/dW and dL/db using subgradient zero at ReLU(0)."""
    if not isinstance(x,(list,tuple)) or not x:
        raise ValueError("nonempty feature vector required")
    inputs=[_finite(v) for v in x]
    if not isinstance(weights,(list,tuple)) or not weights:
        raise ValueError("nonempty weight matrix required")
    if not isinstance(bias,(list,tuple)) or not isinstance(upstream,(list,tuple)) or len(bias)!=len(weights) or len(upstream)!=len(weights):
        raise ValueError("bias and upstream lengths must match output neurons")
    matrix=[]
    for row in weights:
        if not isinstance(row,(list,tuple)) or len(row)!=len(inputs):
            raise ValueError("weights must be rectangular, with input dimension")
        matrix.append([_finite(v) for v in row])
    biases=[_finite(v) for v in bias]
    up=[_finite(v) for v in upstream]
    dx=[0.0]*len(inputs)
    dw=[]
    db=[]
    for j,row in enumerate(matrix):
        z=0.0
        for weight,component in zip(row,inputs):
            product=weight*component
            z+=product
            if not math.isfinite(product) or not math.isfinite(z):
                raise ValueError("forward affine overflow")
        z+=biases[j]
        if not math.isfinite(z):
            raise ValueError("bias addition overflow")
        dz=up[j] if z>0.0 else 0.0
        db.append(dz)
        gradients=[]
        for i,weight in enumerate(row):
            derivative=dz*inputs[i]
            component=weight*dz
            dx[i]+=component
            if not all(map(math.isfinite,(derivative,component,dx[i]))):
                raise ValueError("gradient overflow")
            gradients.append(derivative)
        dw.append(gradients)
    return dx,dw,db
