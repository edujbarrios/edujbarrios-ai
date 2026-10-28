"""Numerically stable log-softmax without materializing raw exponentials."""
import math


def log_softmax(logits):
    """Return normalized log probabilities of one finite logit vector."""
    if not isinstance(logits, (list, tuple)) or not 1 <= len(logits) <= 10000:
        raise ValueError("logit vector length must be in 1..10000")
    x = []
    for value in logits:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("numeric finite logits required")
        try:
            z = float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(z) or abs(z) > 1_000_000:
            raise ValueError("logit outside declared finite bounds")
        x.append(z)
    largest = max(x)
    correction = math.log(sum(math.exp(z-largest) for z in x))
    return [(z-largest)-correction for z in x]
