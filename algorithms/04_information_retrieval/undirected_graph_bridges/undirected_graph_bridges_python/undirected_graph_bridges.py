"""All bridges in a simple undirected graph using DFS low-link values."""


def undirected_graph_bridges(
    adjacency: list[int] | tuple[int, ...], n: int,
) -> list[int]:
    """Return lexicographically sorted, flattened zero-based edge endpoints."""
    if type(n) is not int or not 1 <= n <= 64:
        raise ValueError("vertex count must be an integer in 1..64")
    if (not isinstance(adjacency, (list, tuple))
            or len(adjacency) != n*n
            or any(type(x) is not int or x not in (0, 1) for x in adjacency)):
        raise ValueError("expected flat binary adjacency matrix")
    for u in range(n):
        if adjacency[u*n+u] != 0:
            raise ValueError("self-loops are unsupported")
        for v in range(u):
            if adjacency[u*n+v] != adjacency[v*n+u]:
                raise ValueError("adjacency must be symmetric")
    disc = [0]*n
    low = [0]*n
    parent = [-1]*n
    is_bridge = [False]*(n*n)
    clock = 0

    def dfs(u: int) -> None:
        nonlocal clock
        clock += 1
        disc[u] = low[u] = clock
        for v in range(n):
            if adjacency[u*n+v] == 0:
                continue
            if disc[v] == 0:
                parent[v] = u
                dfs(v)
                low[u] = min(low[u], low[v])
                if low[v] > disc[u]:
                    a, b = min(u,v), max(u,v)
                    is_bridge[a*n+b] = True
            elif v != parent[u]:
                low[u] = min(low[u], disc[v])

    for u in range(n):
        if disc[u] == 0:
            dfs(u)
    result = []
    for u in range(n):
        for v in range(u+1,n):
            if is_bridge[u*n+v]:
                result.extend((u,v))
    return result
