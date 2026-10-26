"""Numerically stable F-beta score from binary-class confusion counts."""
import math


def f_beta_score(tp: int, fp: int, fn: int, beta: float = 1.0) -> float:
    """Return the positive-class F-beta score in [0, 1]."""
    for count in (tp, fp, fn):
        if isinstance(count, bool) or not isinstance(count, int):
            raise ValueError("counts must be nonnegative signed 32-bit integers")
        if not 0 <= count <= 2**31 - 1:
            raise ValueError("counts outside signed 32-bit range")
    if isinstance(beta, bool) or not isinstance(beta, (int, float)):
        raise ValueError("beta must be a positive finite real")
    try:
        b = float(beta)
    except OverflowError as exc:
        raise ValueError("beta is unrepresentable") from exc
    if not math.isfinite(b) or b <= 0.0:
        raise ValueError("beta must be a positive finite real")
    if tp == 0:
        return 0.0
    precision = tp / (tp + fp)
    recall = tp / (tp + fn)
    if b <= 1.0:
        b2 = b * b
        weight = b2 / (1.0 + b2)
    else:
        inverse = 1.0 / b
        weight = 1.0 / (1.0 + inverse * inverse)
    return precision * recall / (weight * precision + (1.0 - weight) * recall)
