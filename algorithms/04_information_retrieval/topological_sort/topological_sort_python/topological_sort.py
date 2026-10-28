"""Deterministic topological ordering of a binary directed graph."""


def topological_sort(weights):
    """Return vertex IDs in a topological order, rejecting directed cycles."""
    if not isinstance(weights, (list, tuple)) or not 1 <= len(weights) <= 128:
        raise ValueError("square directed graph of size 1..128 required")
    n = len(weights)
    graph = []
    for row in weights:
        if not isinstance(row, (list, tuple)) or len(row) != n:
            raise ValueError("adjacency matrix must be square")
        checked = []
        for value in row:
            if isinstance(value, bool) or not isinstance(value, (int, float)) or value not in (0, 1):
                raise ValueError("entries must be 0 or 1")
            checked.append(int(value))
        graph.append(checked)
    indegree = [0] * n
    for row in graph:
        for v, edge in enumerate(row):
            indegree[v] += edge
    used = [False] * n
    output = []
    for _ in range(n):
        candidate = next(
            (v for v in range(n) if not used[v] and indegree[v] == 0),
            None,
        )
        if candidate is None:
            raise ValueError("directed cycle detected")
        output.append(candidate)
        used[candidate] = True
        for v, edge in enumerate(graph[candidate]):
            indegree[v] -= edge
    return output
