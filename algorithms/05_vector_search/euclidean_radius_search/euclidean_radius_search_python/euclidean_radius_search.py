"""Exact Euclidean radius-neighbor retrieval, sorted by distance and row index."""
import math


def euclidean_radius_search(query, candidates, radius):
    """Return all row indices within inclusive L2 radius in distance order."""
    if (isinstance(radius, bool) or not isinstance(radius, (int, float))):
        raise ValueError("radius must be a finite real number")
    try:
        r = float(radius)
    except OverflowError as exc:
        raise ValueError("radius cannot be represented") from exc
    if not math.isfinite(r) or not 0.0 <= r <= 10000.0:
        raise ValueError("radius must be in [0,10000]")
    if not isinstance(query, (list, tuple)) or not 1 <= len(query) <= 32:
        raise ValueError("query dimension must be 1..32")
    if not isinstance(candidates, (list, tuple)) or not 1 <= len(candidates) <= 128:
        raise ValueError("expected 1..128 candidate rows")

    def valid_real(v):
        if isinstance(v, bool) or not isinstance(v, (int, float)):
            raise ValueError("coordinates must be finite real numbers")
        try:
            x = float(v)
        except OverflowError as exc:
            raise ValueError("coordinate outside floating-point range") from exc
        if not math.isfinite(x) or abs(x) > 1000.0:
            raise ValueError("coordinates must be finite in [-1000,1000]")
        return x

    q = [valid_real(x) for x in query]
    rows = []
    for row in candidates:
        if not isinstance(row, (list, tuple)) or len(row) != len(q):
            raise ValueError("candidate rows must match query dimension")
        rows.append([valid_real(x) for x in row])
    matches = []
    for i, row in enumerate(rows):
        distance = math.hypot(*(a - b for a, b in zip(q, row)))
        if distance <= r:
            matches.append((distance, i))
    matches.sort()
    return [i for _, i in matches]
