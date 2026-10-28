"""Bipartite decision by BFS two-coloring of an undirected graph."""
from collections import deque


def bipartite_graph_check(adjacency):
    """Return True iff a symmetric 0/1 adjacency graph is bipartite."""
    if not isinstance(adjacency,(list,tuple)) or not 1<=len(adjacency)<=128:
        raise ValueError("square graph size 1..128 required")
    n=len(adjacency)
    graph=[]
    for row in adjacency:
        if not isinstance(row,(list,tuple)) or len(row)!=n:
            raise ValueError("adjacency must be square")
        checked=[]
        for edge in row:
            if isinstance(edge,bool) or not isinstance(edge,(int,float)) or edge not in (0,1):
                raise ValueError("binary matrix values required")
            checked.append(int(edge))
        graph.append(checked)
    for u in range(n):
        for v in range(u):
            if graph[u][v]!=graph[v][u]:
                raise ValueError("undirected graph must be symmetric")
    color=[-1]*n
    for start in range(n):
        if color[start]!=-1:
            continue
        color[start]=0
        queue=deque([start])
        while queue:
            u=queue.popleft()
            for v,edge in enumerate(graph[u]):
                if not edge:
                    continue
                if color[v]==-1:
                    color[v]=1-color[u]
                    queue.append(v)
                elif color[v]==color[u]:
                    return False
    return True
