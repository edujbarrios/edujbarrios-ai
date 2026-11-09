"""Tarjan DFS articulation vertices of an undirected simple graph."""


def articulation_points(adjacency: list[int] | tuple[int, ...], n: int) -> list[int]:
    """Return sorted zero-based cut vertices of a validated n×n graph."""
    if type(n) is not int or not 1 <= n <= 64:
        raise ValueError("vertex count must be an integer in 1..64")
    if (not isinstance(adjacency, (list, tuple))
            or len(adjacency) != n * n
            or any(type(v) is not int or v not in (0, 1) for v in adjacency)):
        raise ValueError("expected flat binary adjacency matrix")
    for u in range(n):
        if adjacency[u * n + u] != 0:
            raise ValueError("self-loops are unsupported")
        for v in range(u):
            if adjacency[u * n + v] != adjacency[v * n + u]:
                raise ValueError("matrix must be symmetric")
    disc = [0] * n
    low = [0] * n
    parent = [-1] * n
    cut = [False] * n
    clock = 0

    def dfs(u: int) -> None:
        nonlocal clock
        clock += 1
        disc[u] = low[u] = clock
        children = 0
        for v in range(n):
            if not adjacency[u * n + v]:
                continue
            if disc[v] == 0:
                parent[v] = u
                children += 1
                dfs(v)
                low[u] = min(low[u], low[v])
                if parent[u] != -1 and low[v] >= disc[u]:
                    cut[u] = True
            elif v != parent[u]:
                low[u] = min(low[u], disc[v])
        if parent[u] == -1 and children > 1:
            cut[u] = True

    for u in range(n):
        if disc[u] == 0:
            dfs(u)
    return [i for i, is_cut in enumerate(cut) if is_cut]
