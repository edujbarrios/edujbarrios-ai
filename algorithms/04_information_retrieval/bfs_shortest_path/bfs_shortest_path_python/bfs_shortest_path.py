"""Breadth-first fewest-hop distances for directed 0/1 graphs."""
from collections import deque


def bfs_shortest_path(weights, source):
    """Return hop counts from zero-based source, -1 for unreachable."""
    if not isinstance(weights, (list, tuple)) or not 1 <= len(weights) <= 128:
        raise ValueError("square graph of size 1..128 required")
    n = len(weights)
    if isinstance(source, bool) or not isinstance(source, int) or not 0 <= source < n:
        raise ValueError("invalid zero-based source")
    graph = []
    for row in weights:
        if not isinstance(row, (list, tuple)) or len(row) != n:
            raise ValueError("adjacency matrix must be square")
        checked = []
        for value in row:
            if isinstance(value, bool) or not isinstance(value, (int, float)) or value not in (0, 1):
                raise ValueError("adjacency entries must be 0 or 1")
            checked.append(int(value))
        graph.append(checked)
    distances = [-1] * n
    distances[source] = 0
    queue = deque([source])
    while queue:
        u = queue.popleft()
        for v, edge in enumerate(graph[u]):
            if edge and distances[v] == -1:
                distances[v] = distances[u] + 1
                queue.append(v)
    return distances
