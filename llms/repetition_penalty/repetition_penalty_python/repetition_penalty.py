"""Sign-aware decoding penalty applied to logits of previously seen IDs."""
import math


def repetition_penalty(logits, history, penalty):
    """Return a new logit vector; duplicate history IDs count once."""
    if not isinstance(logits, (list, tuple)) or not 1 <= len(logits) <= 4096:
        raise ValueError("logits must be a nonempty vocabulary of at most 4096")
    if not isinstance(history, (list, tuple)) or len(history) > 16384:
        raise ValueError("invalid token history")
    if isinstance(penalty, bool) or not isinstance(penalty, (int, float)):
        raise ValueError("penalty must be numeric")
    try:
        p = float(penalty)
    except OverflowError as exc:
        raise ValueError("unrepresentable penalty") from exc
    if not math.isfinite(p) or not 1 <= p <= 10:
        raise ValueError("penalty outside [1,10]")
    values = []
    for x in logits:
        if isinstance(x, bool) or not isinstance(x, (int, float)):
            raise ValueError("finite numeric logits required")
        try:
            value = float(x)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(value) or abs(value) > 1000:
            raise ValueError("logit outside [-1000,1000]")
        values.append(value)
    seen = set()
    for token in history:
        if (isinstance(token, bool) or not isinstance(token, int) or
                not 0 <= token < len(values)):
            raise ValueError("invalid zero-based token ID")
        seen.add(token)
    for token in seen:
        x = values[token]
        values[token] = x / p if x > 0 else x * p
    return values
