"""Deterministic Euclidean farthest-first traversal for k-center."""
import math
from typing import Sequence

def _real(value: float) -> float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real coordinates required")
    try: number=float(value)
    except OverflowError as exc: raise ValueError("unrepresentable coordinate") from exc
    if not math.isfinite(number):
        raise ValueError("nonfinite coordinate")
    return number

def farthest_first_traversal(points: Sequence[Sequence[float]],k: int) -> list[int]:
    """Select zero-based center indices; start at row zero; break ties by index."""
    if not isinstance(points,(list,tuple)) or not points or not isinstance(points[0],(list,tuple)) or not points[0]:
        raise ValueError("nonempty rectangular point matrix required")
    if isinstance(k,bool) or not isinstance(k,int) or not 1<=k<=len(points):
        raise ValueError("k must be an integer in 1..n")
    d=len(points[0])
    rows=[]
    for row in points:
        if not isinstance(row,(list,tuple)) or len(row)!=d:
            raise ValueError("rectangular point matrix required")
        rows.append([_real(v) for v in row])
    selected=[]
    used=[False]*len(rows)
    nearest=[math.inf]*len(rows)
    for step in range(k):
        if step==0:
            chosen=0
        else:
            chosen=max((i for i in range(len(rows)) if not used[i]),
                       key=lambda i:(nearest[i],-i))
        selected.append(chosen)
        used[chosen]=True
        for i,row in enumerate(rows):
            if used[i]:
                continue
            distance=0.0
            for a,b in zip(row,rows[chosen]):
                delta=a-b
                if not math.isfinite(delta):
                    raise ValueError("coordinate difference overflow")
                distance=math.hypot(distance,delta)
                if not math.isfinite(distance):
                    raise ValueError("Euclidean distance overflow")
            nearest[i]=min(nearest[i],distance)
    return selected
