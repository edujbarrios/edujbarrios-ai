"""Dense Prim minimum spanning tree, returning total weight."""
import math


def prim_mst(weights):
    """Return minimum tree cost for a connected symmetric matrix."""
    if not isinstance(weights, (list, tuple)) or not 1 <= len(weights) <= 128:
        raise ValueError("square graph of size 1..128 required")
    n = len(weights)
    graph = []
    for row in weights:
        if not isinstance(row, (list, tuple)) or len(row) != n:
            raise ValueError("adjacency matrix must be square")
        checked = []
        for value in row:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("numeric edge weights required")
            try:
                w = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable edge weight") from exc
            if not math.isfinite(w) or not 0 <= w <= 1_000_000:
                raise ValueError("weights must be finite and in 0..1000000")
            checked.append(w)
        graph.append(checked)
    for u in range(n):
        if graph[u][u] != 0:
            raise ValueError("diagonal must be zero")
        for v in range(u):
            if graph[u][v] != graph[v][u]:
                raise ValueError("graph must be undirected and symmetric")

    inside = [False] * n
    best = [math.inf] * n
    best[0] = 0.0
    total = 0.0
    for _ in range(n):
        u = None
        for v in range(n):
            if not inside[v] and math.isfinite(best[v]) and (
                u is None or best[v] < best[u]
            ):
                u = v
        if u is None:
            raise ValueError("graph is disconnected")
        inside[u] = True
        total += best[u]
        for v, weight in enumerate(graph[u]):
            if not inside[v] and weight > 0 and weight < best[v]:
                best[v] = weight
    return total
