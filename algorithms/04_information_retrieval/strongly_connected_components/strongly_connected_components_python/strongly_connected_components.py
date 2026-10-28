"""Kosaraju's two-pass SCC search with canonical minimum-vertex labels."""


def strongly_connected_components(adjacency):
    """Return min-vertex SCC label for each vertex of a directed binary graph."""
    if not isinstance(adjacency, (list, tuple)) or not 1 <= len(adjacency) <= 128:
        raise ValueError("square directed graph of size 1..128 required")
    n = len(adjacency)
    graph = []
    for row in adjacency:
        if not isinstance(row, (list, tuple)) or len(row) != n:
            raise ValueError("square binary adjacency matrix required")
        checked = []
        for cell in row:
            if isinstance(cell, bool) or not isinstance(cell, (int, float)) or cell not in (0,1):
                raise ValueError("matrix entries must be zero or one")
            checked.append(int(cell))
        graph.append(checked)

    seen = [False] * n
    order = []

    def finish(u):
        seen[u] = True
        for v, edge in enumerate(graph[u]):
            if edge and not seen[v]:
                finish(v)
        order.append(u)

    for u in range(n):
        if not seen[u]:
            finish(u)
    component = [-1] * n

    def collect(u, label):
        component[u] = label
        for v in range(n):
            if graph[v][u] and component[v] == -1:
                collect(v, label)

    for u in reversed(order):
        if component[u] == -1:
            collect(u, u)
    minima = {}
    for v, group in enumerate(component):
        minima[group] = min(minima.get(group, v), v)
    return [minima[group] for group in component]
