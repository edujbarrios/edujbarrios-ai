"""Gaussian RBF Gram matrix over finite bounded dense feature vectors."""
import math
from typing import Sequence

def gaussian_rbf_kernel(rows:Sequence[Sequence[float]],
                        sigma:float)->list[list[float]]:
    """Return symmetric n-by-n Gaussian kernel matrix with original row order."""
    if not isinstance(rows,(list,tuple)) or not 1<=len(rows)<=128 or        not isinstance(rows[0],(list,tuple)) or not 1<=len(rows[0])<=32:
        raise ValueError("expected n=1..128, d=1..32 matrix")
    if isinstance(sigma,bool) or not isinstance(sigma,(int,float)):
        raise ValueError("finite sigma required")
    try:scale=float(sigma)
    except OverflowError as exc:raise ValueError("unrepresentable scale") from exc
    if not math.isfinite(scale) or not 1e-6<=scale<=1e6:
        raise ValueError("sigma must lie in [1e-6,1e6]")
    n=len(rows);d=len(rows[0])
    data=[]
    for row in rows:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular feature matrix required")
        converted=[]
        for element in row:
            if isinstance(element,bool) or not isinstance(element,(int,float)):
                raise ValueError("finite real coordinate required")
            try:value=float(element)
            except OverflowError as exc:raise ValueError("unrepresentable coordinate") from exc
            if not math.isfinite(value) or abs(value)>1e6:
                raise ValueError("coordinate outside supported bounds")
            converted.append(value)
        data.append(converted)
    output=[[0.0]*n for _ in range(n)]
    for i in range(n):
        for j in range(i,n):
            distance=0.0
            for a,b in zip(data[i],data[j]):
                distance=math.hypot(distance,a-b)
            scaled=distance/scale
            value=math.exp(-.5*scaled*scaled)
            if not math.isfinite(value) or not 0<=value<=1:
                raise ValueError("Gaussian kernel overflow")
            output[i][j]=output[j][i]=value
    return output
