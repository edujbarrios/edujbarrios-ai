"""Numerically stable temperature-scaled next-token softmax."""
import math


def temperature_softmax(logits, temperature):
    """Return a probability distribution for positive temperature."""
    if not isinstance(logits, (list, tuple)) or not 1 <= len(logits) <= 4096:
        raise ValueError("logit vector length must be 1..4096")
    if isinstance(temperature, bool) or not isinstance(temperature, (int, float)):
        raise ValueError("temperature must be numeric")
    try:
        t = float(temperature)
    except OverflowError as exc:
        raise ValueError("unrepresentable temperature") from exc
    if not math.isfinite(t) or not 0.01 <= t <= 100:
        raise ValueError("temperature must lie in [0.01,100]")
    values = []
    for value in logits:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("logits must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(number) or abs(number) > 1000:
            raise ValueError("invalid logit")
        values.append(number)
    maximum = max(values)
    weights = [math.exp((v - maximum) / t) for v in values]
    total = sum(weights)
    return [w / total for w in weights]
