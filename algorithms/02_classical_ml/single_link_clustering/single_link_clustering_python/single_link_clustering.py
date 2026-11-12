"""Deterministic single-link clustering by minimum cross-group point distance."""


def single_link_clustering(points: list[int], n: int, k: int) -> list[int]:
    """Return k cluster labels for n row-major integer 2D points."""
    if type(n) is not int or type(k) is not int or not 1 <= n <= 32 or \
            not 1 <= k <= n or not isinstance(points, (list, tuple)) or \
            len(points) != 2 * n:
        raise ValueError("n 1..32, k 1..n, and 2*n integer coordinates required")
    if any(type(x) is not int or not -1000 <= x <= 1000 for x in points):
        raise ValueError("coordinates must be bounded integers")

    groups = list(range(n))
    for _ in range(n - k):
        closest = None
        best_i, best_j = 0, 0
        for i in range(n):
            for j in range(i + 1, n):
                if groups[i] == groups[j]:
                    continue
                dx = points[2*i] - points[2*j]
                dy = points[2*i+1] - points[2*j+1]
                candidate = (dx*dx + dy*dy, i, j)
                if closest is None or candidate < closest:
                    closest = candidate
                    best_i, best_j = i, j
        old, new = groups[best_j], groups[best_i]
        groups = [new if group == old else group for group in groups]
    mapping: dict[int, int] = {}
    return [mapping.setdefault(group, len(mapping)) for group in groups]
