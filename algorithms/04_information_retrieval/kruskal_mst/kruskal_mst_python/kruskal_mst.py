"""Kruskal minimum spanning tree for a bounded dense undirected graph."""
import math
from typing import Sequence

def kruskal_mst(weights:Sequence[Sequence[float]])->tuple[list[tuple[int,int]],float]:
    """Return (u,v) edges selected in deterministic greedy order and total cost."""
    if not isinstance(weights,(list,tuple)) or not 1<=len(weights)<=64:
        raise ValueError("expected a square adjacency matrix with n=1..64")
    n=len(weights)
    matrix=[]
    for row in weights:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("adjacency matrix must be square")
        data=[]
        for item in row:
            if isinstance(item,bool) or not isinstance(item,(int,float)):
                raise ValueError("edge weights must be numeric")
            try:value=float(item)
            except OverflowError as exc:raise ValueError("unrepresentable edge") from exc
            if math.isnan(value) or value==-math.inf or (math.isfinite(value) and not 0<=value<=1e6):
                raise ValueError("edge weight must be nonnegative and finite or +Inf")
            data.append(value)
        matrix.append(data)
    if any(matrix[i][i]!=0 for i in range(n)):
        raise ValueError("diagonal must equal zero")
    edges=[]
    for i in range(n):
        for j in range(i+1,n):
            if matrix[i][j]!=matrix[j][i]:
                raise ValueError("edge weights must be exactly symmetric")
            if math.isfinite(matrix[i][j]):
                edges.append((matrix[i][j],i,j))
    edges.sort()
    parent=list(range(n))
    sizes=[1]*n
    def root(x):
        while parent[x]!=x:
            parent[x]=parent[parent[x]]
            x=parent[x]
        return x
    selected=[]
    total=0.0
    for weight,i,j in edges:
        a,b=root(i),root(j)
        if a==b:continue
        if sizes[a]<sizes[b]:a,b=b,a
        parent[b]=a
        sizes[a]+=sizes[b]
        selected.append((i,j))
        total+=weight
        if not math.isfinite(total):
            raise ValueError("MST sum overflow")
        if len(selected)==n-1:break
    if len(selected)!=n-1:
        raise ValueError("graph is disconnected")
    return selected,total
