"""Sample-weighted binary Brier score for probabilistic predictions."""
import math
from typing import Sequence


def weighted_brier_score(targets: Sequence[float], probabilities: Sequence[float],
                         weights: Sequence[float]) -> float:
    """Return weighted binary Brier score, or raise ValueError."""
    n = len(targets)
    if not 1 <= n <= 4096 or len(probabilities) != n or len(weights) != n:
        raise ValueError("targets, probabilities and weights must have same length 1..4096")
    error = total_weight = 0.0
    for a, b, c in zip(targets, probabilities, weights):
        if any(isinstance(x, bool) or not isinstance(x, (int, float))
               for x in (a, b, c)):
            raise ValueError("all values must be real numbers")
        try:
            y, p, w = float(a), float(b), float(c)
        except OverflowError as exc:
            raise ValueError("value is not representable") from exc
        if (not math.isfinite(y) or not math.isfinite(p) or
                not math.isfinite(w) or y not in (0.0, 1.0) or
                not 0 <= p <= 1 or not 0 <= w <= 1_000_000):
            raise ValueError("invalid binary target, probability or sample weight")
        error += w * (p - y) ** 2
        total_weight += w
    if total_weight <= 0:
        raise ValueError("sample weights must sum to a positive value")
    return error / total_weight
