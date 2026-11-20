"""Empirical binary mutual information (bits) from a 2x2 count table."""
import math


def binary_mutual_information(x, y) -> float:
    """Calculate I(X;Y) with zero joint-probability terms skipped."""
    if (not isinstance(x, (list, tuple)) or not isinstance(y, (list, tuple))
            or not x or len(x) != len(y)):
        raise ValueError("nonempty aligned binary vectors required")
    counts = [[0, 0], [0, 0]]
    for a, b in zip(x, y):
        if (isinstance(a, bool) or not isinstance(a, int) or a not in (0, 1)
                or isinstance(b, bool) or not isinstance(b, int) or b not in (0, 1)):
            raise ValueError("binary integer values required")
        counts[a][b] += 1
    n = len(x)
    rows = [sum(row) for row in counts]
    cols = [counts[0][j] + counts[1][j] for j in range(2)]
    result = 0.0
    for a in range(2):
        for b in range(2):
            count = counts[a][b]
            if count:
                p = count / n
                result += p * math.log2(p / ((rows[a] / n) * (cols[b] / n)))
    if not math.isfinite(result):
        raise ValueError("nonfinite mutual information")
    return max(0.0, result)
