"""Deterministic IVF-Flat style coarse partitioning and local exact scanning."""

import math


def _vector(values, name, dimensions=None):
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 32:
        raise ValueError(f"{name} must contain 1..32 coordinates")
    if dimensions is not None and len(values) != dimensions:
        raise ValueError(f"{name} has inconsistent dimensionality")
    normalized = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError(f"{name} must contain bounded finite reals")
        try:
            number = float(value)
        except (ValueError, OverflowError):
            raise ValueError(f"{name} must contain bounded finite reals") from None
        if not math.isfinite(number) or abs(number) > 1_000_000:
            raise ValueError(f"{name} must contain bounded finite reals")
        normalized.append(number)
    return tuple(normalized)


def _squared_distance(a, b):
    return sum((x - y) * (x - y) for x, y in zip(a, b))


def ivf_flat_search(points, query, n_clusters, n_probe, k):
    """Return at most k neighbors from the n_probe nearest coarse cells."""
    if not isinstance(points, (list, tuple)) or not 1 <= len(points) <= 4096:
        raise ValueError("points must contain 1..4096 vectors")
    n = len(points)
    for value, bound, name in (
        (n_clusters, n, "n_clusters"),
        (n_probe, n_clusters, "n_probe"),
        (k, n, "k"),
    ):
        if isinstance(value, bool) or not isinstance(value, int) or not 1 <= value <= bound:
            raise ValueError(f"{name} must be a positive integer within its bound")

    vectors = []
    for row in points:
        vectors.append(_vector(row, "point", len(vectors[0]) if vectors else None))
    target = _vector(query, "query", len(vectors[0]))

    center_ids = [0]
    used = {0}
    min_distances = [_squared_distance(vector, vectors[0]) for vector in vectors]
    while len(center_ids) < n_clusters:
        remaining = (i for i in range(n) if i not in used)
        selected = max(remaining, key=lambda i: (min_distances[i], -i))
        if min_distances[selected] == 0:
            raise ValueError("n_clusters exceeds the number of distinct points")
        center_ids.append(selected)
        used.add(selected)
        for i, vector in enumerate(vectors):
            min_distances[i] = min(min_distances[i],
                                   _squared_distance(vector, vectors[selected]))

    inverted_lists = [[] for _ in center_ids]
    for i, vector in enumerate(vectors):
        center = min(range(n_clusters),
                     key=lambda j: (_squared_distance(vector, vectors[center_ids[j]]), j))
        inverted_lists[center].append(i)

    ranked_cells = sorted(range(n_clusters),
                          key=lambda j: (_squared_distance(target, vectors[center_ids[j]]), j))
    candidate_ids = [i for cell in ranked_cells[:n_probe] for i in inverted_lists[cell]]
    ranked = sorted(((i, _squared_distance(target, vectors[i]))
                     for i in candidate_ids), key=lambda pair: (pair[1], pair[0]))
    return ranked[:k]
