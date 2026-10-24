"""Dense deterministic Dijkstra single-source shortest paths."""
import math
from typing import Sequence

def dijkstra_shortest_path(weights:Sequence[Sequence[float]],source:int)->list[float]:
    """Return distances from zero-based source; unreachable nodes use -1."""
    if not isinstance(weights,(list,tuple)) or not 1<=len(weights)<=128:
        raise ValueError("square graph of size 1..128 required")
    n=len(weights)
    if isinstance(source,bool) or not isinstance(source,int) or not 0<=source<n:
        raise ValueError("source index out of range")
    data=[]
    for row in weights:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("graph adjacency must be square")
        checked=[]
        for x in row:
            if isinstance(x,bool) or not isinstance(x,(int,float)):
                raise ValueError("finite real edge weights required")
            try:v=float(x)
            except OverflowError as exc:raise ValueError("unrepresentable weight") from exc
            if not math.isfinite(v) or not 0<=v<=1_000_000:
                raise ValueError("weight outside 0..1000000")
            checked.append(v)
        data.append(checked)
    distances=[math.inf]*n
    visited=[False]*n
    distances[source]=0.0
    for _ in range(n):
        u=None
        for i in range(n):
            if not visited[i] and math.isfinite(distances[i]) and (
                u is None or distances[i]<distances[u]
            ): u=i
        if u is None: break
        visited[u]=True
        for v,w in enumerate(data[u]):
            if w==0 or visited[v]:continue
            candidate=distances[u]+w
            if not math.isfinite(candidate):raise ValueError("distance overflow")
            if candidate<distances[v]:distances[v]=candidate
    return [x if math.isfinite(x) else -1.0 for x in distances]
