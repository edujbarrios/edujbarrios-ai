"""Univariate equal-weight isotonic regression via pool-adjacent-violators."""
import math


def isotonic_regression(values: list[float]) -> list[float]:
    """Return a nondecreasing least-squares fit in the original sample order."""
    if not isinstance(values, (list, tuple)) or not 1 <= len(values) <= 512:
        raise ValueError("1..512 ordered observations required")
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (float, int)) or \
                not math.isfinite(value) or abs(value) > 1_000_000:
            raise ValueError("bounded finite numeric observation required")

    means: list[float] = []
    counts: list[int] = []
    for value in values:
        means.append(float(value))
        counts.append(1)
        while len(means) > 1 and means[-2] > means[-1]:
            count = counts[-2] + counts[-1]
            means[-2] = (means[-2] * counts[-2] + means[-1] * counts[-1]) / count
            counts[-2] = count
            means.pop()
            counts.pop()
    return [mean for mean, count in zip(means, counts) for _ in range(count)]
