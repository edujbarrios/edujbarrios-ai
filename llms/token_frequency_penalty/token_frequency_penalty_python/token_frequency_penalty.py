"""Occurrence-count based next-token frequency penalty."""
import math


def token_frequency_penalty(logits, history, penalty):
    """Subtract penalty*count[token_id] from each next-token logit."""
    if not isinstance(logits, (list, tuple)) or not 1 <= len(logits) <= 4096:
        raise ValueError("logit vocabulary must have 1..4096 entries")
    if not isinstance(history, (list, tuple)) or len(history)>4096:
        raise ValueError("history length must be 0..4096")
    if isinstance(penalty, bool) or not isinstance(penalty, (int, float)):
        raise ValueError("penalty must be numeric")
    try:
        a = float(penalty)
    except OverflowError as exc:
        raise ValueError("unrepresentable penalty") from exc
    if not math.isfinite(a) or not 0 <= a <= 5:
        raise ValueError("penalty must be finite in [0,5]")
    values = []
    for raw in logits:
        if isinstance(raw, bool) or not isinstance(raw, (int, float)):
            raise ValueError("numeric logits required")
        try:
            v = float(raw)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(v) or abs(v)>1000:
            raise ValueError("logit out of range")
        values.append(v)
    counts = [0]*len(values)
    for i in history:
        if isinstance(i, bool) or not isinstance(i, int) or not 0 <= i < len(values):
            raise ValueError("invalid zero-based token ID")
        counts[i] += 1
    return [score-a*count for score,count in zip(values,counts)]
