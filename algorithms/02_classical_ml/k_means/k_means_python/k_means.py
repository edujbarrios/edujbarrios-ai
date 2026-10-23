"""Deterministic dense K-means (Lloyd's algorithm), implemented from scratch."""
import math
from typing import Sequence


def k_means(samples: Sequence[Sequence[float]], k: int,
            max_iterations: int) -> tuple[list[list[float]], list[int]]:
    """Return (centroids, zero-based assignments) for finite rectangular input."""
    if not isinstance(samples, (list, tuple)) or not samples:
        raise ValueError("samples must be a nonempty matrix")
    if not isinstance(k, int) or isinstance(k, bool) or not 1 <= k <= len(samples):
        raise ValueError("k must be an integer in 1..sample count")
    if isinstance(max_iterations, bool) or not isinstance(max_iterations, int) or max_iterations < 1:
        raise ValueError("max_iterations must be a positive integer")

    d = len(samples[0]) if isinstance(samples[0], (list, tuple)) else 0
    if d < 1:
        raise ValueError("samples must contain at least one feature")
    points = []
    for row in samples:
        if not isinstance(row, (list, tuple)) or len(row) != d:
            raise ValueError("samples must be rectangular")
        values = []
        for value in row:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("features must be real numbers")
            try:
                converted = float(value)
            except OverflowError as exc:
                raise ValueError("features must be finite") from exc
            if not math.isfinite(converted):
                raise ValueError("features must be finite")
            values.append(converted)
        points.append(values)

    centers = [row.copy() for row in points[:k]]
    for index in range(k):
        if centers[index] in centers[:index]:
            raise ValueError("first k samples must be pairwise distinct")

    labels = [-1] * len(points)
    for _ in range(max_iterations):
        new_labels = []
        sums = [[0.0] * d for _ in range(k)]
        counts = [0] * k

        for point in points:
            best_index, best_distance = 0, math.inf
            for cluster, center in enumerate(centers):
                distance = 0.0
                for coordinate, reference in zip(point, center):
                    difference = coordinate - reference
                    if not math.isfinite(difference):
                        raise ValueError("distance overflow")
                    distance = math.hypot(distance, difference)
                    if not math.isfinite(distance):
                        raise ValueError("distance overflow")
                if distance < best_distance:
                    best_distance, best_index = distance, cluster
            new_labels.append(best_index)
            counts[best_index] += 1
            for col in range(d):
                sums[best_index][col] += point[col]
                if not math.isfinite(sums[best_index][col]):
                    raise ValueError("centroid sum overflow")

        next_centers = [row.copy() for row in centers]
        for cluster in range(k):
            if counts[cluster]:
                next_centers[cluster] = [value / counts[cluster]
                                         for value in sums[cluster]]
                if not all(math.isfinite(v) for v in next_centers[cluster]):
                    raise ValueError("centroid overflow")

        unchanged = new_labels == labels
        labels = new_labels
        centers = next_centers
        if unchanged:
            break
    return centers, labels
