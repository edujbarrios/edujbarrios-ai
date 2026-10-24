"""A* on a directed weighted graph with a validated consistent heuristic."""
import math
from typing import Sequence

def _number(value:float)->float:
    if isinstance(value,bool) or not isinstance(value,(int,float)):
        raise ValueError("finite real graph values required")
    try: x=float(value)
    except OverflowError as exc:raise ValueError("unrepresentable value") from exc
    if not math.isfinite(x) or not 0<=x<=1_000_000:
        raise ValueError("graph values must be finite in [0,1e6]")
    return x

def astar_shortest_path(
    weights:Sequence[Sequence[float]],
    heuristic:Sequence[float],
    source:int,target:int
)->tuple[float,list[int]]:
    """Return (shortest distance, zero-based path), or (-1,[]) if unreachable."""
    if not isinstance(weights,(list,tuple)) or not 1<=len(weights)<=64:
        raise ValueError("1..64 graph rows required")
    n=len(weights)
    if (not isinstance(heuristic,(list,tuple)) or len(heuristic)!=n or
        isinstance(source,bool) or not isinstance(source,int) or
        isinstance(target,bool) or not isinstance(target,int) or
        not 0<=source<n or not 0<=target<n):
        raise ValueError("invalid heuristic or source/target")
    h=[_number(x) for x in heuristic]
    if h[target]!=0:raise ValueError("target heuristic must be zero")
    graph=[]
    for row in weights:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("square graph required")
        graph.append([_number(x) for x in row])
    for u in range(n):
        for v in range(n):
            if graph[u][v]>0 and h[u]>graph[u][v]+h[v]+1e-9:
                raise ValueError("heuristic is inconsistent")
    g=[math.inf]*n
    parent=[-1]*n
    closed=[False]*n
    g[source]=0.0
    for _ in range(n):
        best=None
        for i in range(n):
            if closed[i] or not math.isfinite(g[i]):continue
            f=g[i]+h[i]
            if not math.isfinite(f):raise ValueError("A* priority overflow")
            if best is None or f<best[0]:
                best=(f,i)
        if best is None:return -1.0,[]
        current=best[1]
        if current==target:
            path=[]
            node=target
            while len(path)<n:
                path.append(node)
                if node==source:break
                node=parent[node]
                if node<0:raise ValueError("broken predecessor path")
            if path[-1]!=source:raise ValueError("cyclic predecessor path")
            return g[target],list(reversed(path))
        closed[current]=True
        for v,w in enumerate(graph[current]):
            if not w or closed[v]:continue
            tentative=g[current]+w
            if not math.isfinite(tentative):raise ValueError("A* path overflow")
            if tentative<g[v]:
                g[v]=tentative
                parent[v]=current
    return -1.0,[]
