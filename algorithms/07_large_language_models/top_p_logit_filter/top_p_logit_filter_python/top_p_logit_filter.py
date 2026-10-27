"""Keep the minimum probability-mass prefix of next-token logits."""
import math


def top_p_logit_filter(logits, top_p):
    """Return a boolean vocabulary mask, with stable original-index ties."""
    if not isinstance(logits, (list, tuple)) or not 1 <= len(logits) <= 4096:
        raise ValueError("logits must contain 1..4096 scores")
    if isinstance(top_p, bool) or not isinstance(top_p, (int, float)):
        raise ValueError("top_p must be numeric")
    try:
        threshold = float(top_p)
    except OverflowError as exc:
        raise ValueError("invalid top_p") from exc
    if not math.isfinite(threshold) or not 0 < threshold <= 1:
        raise ValueError("top_p must lie in (0,1]")
    values = []
    for value in logits:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("numeric logits required")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(number) or abs(number) > 1000:
            raise ValueError("invalid logit value")
        values.append(number)
    maximum = max(values)
    weights = [math.exp(value-maximum) for value in values]
    total = sum(weights)
    order = sorted(range(len(values)), key=lambda i: (-values[i], i))
    keep = [False] * len(values)
    cumulative = 0.0
    for i in order:
        keep[i] = True
        cumulative += weights[i] / total
        if threshold < 1 and cumulative >= threshold:
            break
    return keep
