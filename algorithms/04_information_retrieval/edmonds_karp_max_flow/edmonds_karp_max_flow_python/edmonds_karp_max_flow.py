"""Edmonds-Karp maximum directed flow with residual reverse capacities."""
from collections import deque


def edmonds_karp_max_flow(
    capacities: list[int] | tuple[int, ...], n: int, source: int, sink: int
) -> int:
    """Return maximum integral flow from source to sink in a small network."""
    if type(n) is not int or not 2 <= n <= 16:
        raise ValueError("vertex count must be integer 2..16")
    if (type(source) is not int or type(sink) is not int or
            not 0 <= source < n or not 0 <= sink < n or source == sink):
        raise ValueError("expected distinct valid source and sink vertices")
    if (not isinstance(capacities, (list, tuple)) or len(capacities) != n*n or
            any(type(x) is not int or not 0 <= x <= 1000 for x in capacities)):
        raise ValueError("expected flat integer capacity matrix in 0..1000")
    if any(capacities[i*n+i] != 0 for i in range(n)):
        raise ValueError("self-loop capacity is not supported")
    residual = list(capacities)
    result = 0
    while True:
        parent = [-1]*n
        parent[source] = source
        queue = deque([source])
        while queue and parent[sink] < 0:
            u = queue.popleft()
            for v in range(n):
                if residual[u*n+v] > 0 and parent[v] < 0:
                    parent[v] = u
                    queue.append(v)
        if parent[sink] < 0:
            return result
        bottleneck = 1000
        v = sink
        while v != source:
            u = parent[v]
            bottleneck = min(bottleneck,residual[u*n+v])
            v = u
        v = sink
        while v != source:
            u = parent[v]
            residual[u*n+v] -= bottleneck
            residual[v*n+u] += bottleneck
            v = u
        result += bottleneck
