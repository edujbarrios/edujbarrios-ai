"""Relative-probability min-p mask for next-token logits."""
import math


def min_p_logit_filter(logits, min_p):
    """Keep token i iff softmax(logits)[i] >= min_p * max(softmax)."""
    if not isinstance(logits, (list, tuple)) or not 1 <= len(logits) <= 4096:
        raise ValueError("vocabulary size must be 1..4096")
    if isinstance(min_p, bool) or not isinstance(min_p, (int, float)):
        raise ValueError("min_p must be a number in [0,1]")
    try:
        threshold = float(min_p)
    except OverflowError as exc:
        raise ValueError("unrepresentable min_p") from exc
    if not math.isfinite(threshold) or not 0 <= threshold <= 1:
        raise ValueError("min_p must be in [0,1]")
    values = []
    for v in logits:
        if isinstance(v, bool) or not isinstance(v, (int, float)):
            raise ValueError("logits must be finite real numbers")
        try:
            value = float(v)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(value) or abs(value) > 1000:
            raise ValueError("logit outside [-1000,1000]")
        values.append(value)
    top = max(values)
    return [threshold == 0 or math.exp(x - top) >= threshold for x in values]
