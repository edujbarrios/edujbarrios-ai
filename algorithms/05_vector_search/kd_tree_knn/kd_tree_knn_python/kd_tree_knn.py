"""Deterministic, exact Euclidean k-NN using a balanced median-split k-d tree."""

import heapq
import math


def _vector(values, name, dimension=None):
    if not isinstance(values, (tuple, list)) or not 1 <= len(values) <= 32:
        raise ValueError(f"{name} must contain 1..32 coordinates")
    if dimension is not None and len(values) != dimension:
        raise ValueError(f"{name} has inconsistent dimensions")
    converted = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError(f"{name} coordinates must be bounded finite reals")
        try:
            number = float(value)
        except (OverflowError, ValueError):
            raise ValueError(f"{name} coordinates must be bounded finite reals") from None
        if not math.isfinite(number) or abs(number) > 1_000_000:
            raise ValueError(f"{name} coordinates must be bounded finite reals")
        converted.append(number)
    return tuple(converted)


def kd_tree_knn(points, query, k):
    """Return exact (zero-based index, squared L2 distance) pairs.

    Equal-distance neighbors are ordered by the original input index.
    No input is modified; the median tree is rebuilt on each invocation.
    """
    if not isinstance(points, (tuple, list)) or not 1 <= len(points) <= 4096:
        raise ValueError("points must be a nonempty sequence of <=4096 vectors")
    if isinstance(k, bool) or not isinstance(k, int) or not 1 <= k <= len(points):
        raise ValueError("k must be a positive integer <= number of points")
    data = []
    for index, vector in enumerate(points):
        data.append(_vector(vector, f"point {index}",
                            len(data[0]) if data else None))
    target = _vector(query, "query", len(data[0]))
    dimensions = len(target)

    def build(indices, depth):
        if not indices:
            return None
        axis = depth % dimensions
        indices.sort(key=lambda i: (data[i][axis], i))
        mid = len(indices) // 2
        return (indices[mid], axis,
                build(indices[:mid], depth + 1),
                build(indices[mid + 1:], depth + 1))

    root = build(list(range(len(data))), 0)
    best = []  # (-squared_distance, -original_index), worst at heap root.

    def visit(node):
        if node is None:
            return
        index, axis, left, right = node
        distance = sum((a - b) ** 2 for a, b in zip(data[index], target))
        candidate = (-distance, -index)
        if len(best) < k:
            heapq.heappush(best, candidate)
        elif candidate > best[0]:
            heapq.heapreplace(best, candidate)

        delta = target[axis] - data[index][axis]
        near, far = (left, right) if delta <= 0 else (right, left)
        visit(near)
        # Equality is necessary to find lower-index neighbors in a tie.
        if len(best) < k or delta * delta <= -best[0][0]:
            visit(far)

    visit(root)
    return sorted(((-neg_index, -neg_distance)
                   for neg_distance, neg_index in best),
                  key=lambda item: (item[1], item[0]))
