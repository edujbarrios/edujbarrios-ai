"""Edge-list Bellman-Ford shortest paths with reachable negative-cycle detection."""
import math


def bellman_ford_shortest_path(edges, n, source):
    """Return source distances, infinity for unreachable vertices."""
    if isinstance(n, bool) or not isinstance(n, int) or not 1 <= n <= 128:
        raise ValueError("n must be an integer in 1..128")
    if isinstance(source, bool) or not isinstance(source, int) or not 0 <= source < n:
        raise ValueError("source out of range")
    if not isinstance(edges, (list, tuple)) or len(edges) > 16_384:
        raise ValueError("up to 16384 edges required")
    checked = []
    for item in edges:
        if not isinstance(item, (list, tuple)) or len(item) != 3:
            raise ValueError("each edge must have three entries")
        u, v, w = item
        if (isinstance(u, bool) or not isinstance(u, int) or not 0 <= u < n or
                isinstance(v, bool) or not isinstance(v, int) or not 0 <= v < n):
            raise ValueError("invalid vertex index")
        if isinstance(w, bool) or not isinstance(w, (int, float)):
            raise ValueError("numeric edge weight required")
        try:
            weight = float(w)
        except OverflowError as exc:
            raise ValueError("unrepresentable edge weight") from exc
        if not math.isfinite(weight) or abs(weight) > 1_000_000:
            raise ValueError("edge weight outside allowed range")
        checked.append((u, v, weight))
    dist = [math.inf] * n
    dist[source] = 0.0
    for _ in range(n - 1):
        changed = False
        for u, v, w in checked:
            if math.isinf(dist[u]):
                continue
            candidate = dist[u] + w
            if not math.isfinite(candidate):
                raise ValueError("distance overflow")
            if candidate < dist[v]:
                dist[v] = candidate
                changed = True
        if not changed:
            break
    for u, v, w in checked:
        if math.isfinite(dist[u]) and dist[u] + w < dist[v]:
            raise ValueError("reachable negative cycle")
    return dist
