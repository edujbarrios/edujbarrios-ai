"""HITS hub and authority link scores via 100 fixed power iterations."""
import math
from typing import Sequence

def hits_hubs_authorities(weights:Sequence[Sequence[float]]
                          )->tuple[list[float],list[float]]:
    """Return (authorities, hubs), both L2 normalized or all zeros."""
    if not isinstance(weights,(list,tuple)) or not 1<=len(weights)<=64:
        raise ValueError("expected n=1..64 directed adjacency matrix")
    n=len(weights)
    matrix=[]
    for row in weights:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("square adjacency matrix required")
        converted=[]
        for item in row:
            if isinstance(item,bool) or not isinstance(item,(int,float)):
                raise ValueError("finite real weights required")
            try: v=float(item)
            except OverflowError as exc:raise ValueError("unrepresentable edge") from exc
            if not math.isfinite(v) or not 0<=v<=1:
                raise ValueError("weights must lie in [0,1]")
            converted.append(v)
        matrix.append(converted)
    hubs=[1/math.sqrt(n)]*n
    authorities=[0.0]*n
    for _ in range(100):
        raw=[0.0]*n
        for j in range(n):
            for i in range(n):
                raw[j]+=matrix[i][j]*hubs[i]
        if not all(map(math.isfinite,raw)):
            raise ValueError("authority update overflow")
        norm=math.hypot(*raw)
        if not math.isfinite(norm):
            raise ValueError("authority norm overflow")
        authorities=[v/norm if norm else 0.0 for v in raw]
        raw=[0.0]*n
        for i in range(n):
            for j in range(n):
                raw[i]+=matrix[i][j]*authorities[j]
        if not all(map(math.isfinite,raw)):
            raise ValueError("hub update overflow")
        norm=math.hypot(*raw)
        if not math.isfinite(norm):
            raise ValueError("hub norm overflow")
        hubs=[v/norm if norm else 0.0 for v in raw]
    return authorities,hubs
