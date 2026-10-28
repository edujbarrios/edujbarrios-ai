"""Minimum-vertex labels of undirected connected components."""
from collections import deque


def undirected_connected_components(adjacency):
    """Label each component by its smallest zero-based vertex ID."""
    if not isinstance(adjacency,(list,tuple)) or not 1<=len(adjacency)<=128:
        raise ValueError("square adjacency matrix of size 1..128 required")
    n=len(adjacency)
    graph=[]
    for row in adjacency:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("adjacency matrix must be square")
        checked=[]
        for cell in row:
            if isinstance(cell,bool) or not isinstance(cell,(int,float)) or cell not in (0,1):
                raise ValueError("binary adjacency entries required")
            checked.append(int(cell))
        graph.append(checked)
    for u in range(n):
        for v in range(u):
            if graph[u][v]!=graph[v][u]:
                raise ValueError("undirected adjacency must be symmetric")
    labels=[-1]*n
    for start in range(n):
        if labels[start]>=0:
            continue
        labels[start]=start
        queue=deque([start])
        while queue:
            u=queue.popleft()
            for v,edge in enumerate(graph[u]):
                if edge and labels[v]==-1:
                    labels[v]=start
                    queue.append(v)
    return labels
