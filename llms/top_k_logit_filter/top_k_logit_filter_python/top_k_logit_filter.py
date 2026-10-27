"""Deterministic boolean top-k mask for next-token logits."""
import math


def top_k_logit_filter(logits, k):
    """Return a V-element keep mask sorted by (-logit, token_id)."""
    if not isinstance(logits, (list, tuple)) or not 1 <= len(logits) <= 4096:
        raise ValueError("logits length must be 1..4096")
    if isinstance(k, bool) or not isinstance(k, int) or not 1 <= k <= len(logits):
        raise ValueError("k must be an integer in [1, vocabulary]")
    values = []
    for value in logits:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("logits must be numeric")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(number) or abs(number) > 1000:
            raise ValueError("nonfinite or out-of-range logit")
        values.append(number)
    winners = sorted(range(len(values)), key=lambda i: (-values[i], i))[:k]
    mask = [False] * len(values)
    for i in winners:
        mask[i] = True
    return mask
