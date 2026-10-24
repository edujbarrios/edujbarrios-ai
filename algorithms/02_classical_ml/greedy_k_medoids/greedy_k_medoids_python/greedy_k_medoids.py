"""Greedy k-medoid BUILD selection minimizing nearest-representative distance."""
import math
from typing import Sequence

def greedy_k_medoids(points:Sequence[Sequence[float]],k:int)->list[int]:
    """Return distinct medoid row indices in greedy selection order."""
    if (not isinstance(points,(list,tuple)) or
        not 1<=len(points)<=128 or not isinstance(points[0],(list,tuple)) or
        not 1<=len(points[0])<=32):
        raise ValueError("matrix dimensions must satisfy n=1..128,d=1..32")
    n=len(points);d=len(points[0])
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=n:
        raise ValueError("k must be integer 1..n")
    data=[]
    for row in points:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular matrix required")
        values=[]
        for x in row:
            if isinstance(x,bool) or not isinstance(x,(int,float)):
                raise ValueError("finite real coordinates required")
            try:real=float(x)
            except OverflowError as exc:raise ValueError("unrepresentable coordinate") from exc
            if not math.isfinite(real):raise ValueError("nonfinite feature")
            values.append(real)
        data.append(values)
    distances=[[0.0]*n for _ in range(n)]
    for i in range(n):
        for j in range(i,n):
            metric=0.0
            for a,b in zip(data[i],data[j]):
                delta=a-b
                if not math.isfinite(delta):raise ValueError("distance delta overflow")
                metric=math.hypot(metric,delta)
                if not math.isfinite(metric):raise ValueError("distance overflow")
            distances[i][j]=distances[j][i]=metric
    nearest=[math.inf]*n
    selected=[]
    for _ in range(k):
        best=None
        for candidate in range(n):
            if candidate in selected:continue
            cost=0.0
            for i in range(n):
                cost+=min(nearest[i],distances[i][candidate])
                if not math.isfinite(cost):raise ValueError("objective overflow")
            if best is None or cost<best[0]:
                best=(cost,candidate)
        chosen=best[1]
        selected.append(chosen)
        for i in range(n):
            nearest[i]=min(nearest[i],distances[i][chosen])
    return selected
