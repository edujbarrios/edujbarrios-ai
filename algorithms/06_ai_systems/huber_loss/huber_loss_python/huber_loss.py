"""Mean Huber loss for regression with a finite positive transition."""
import math


def huber_loss(actual, predicted, delta=1.0):
    """Compute finite mean Huber loss, rejecting invalid inputs and overflow."""
    if isinstance(delta, bool) or not isinstance(delta, (int, float)):
        raise ValueError("positive finite delta required")
    try:
        d = float(delta)
    except OverflowError as exc:
        raise ValueError("unrepresentable delta") from exc
    if not math.isfinite(d) or d <= 0:
        raise ValueError("positive finite delta required")

    def check(seq):
        if not isinstance(seq, (list, tuple)) or not seq:
            raise ValueError("nonempty real vector required")
        result = []
        for value in seq:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("finite real values required")
            try:
                numeric = float(value)
            except OverflowError as exc:
                raise ValueError("unrepresentable value") from exc
            if not math.isfinite(numeric):
                raise ValueError("finite real values required")
            result.append(numeric)
        return result

    y, yhat = check(actual), check(predicted)
    if len(y) != len(yhat):
        raise ValueError("vector lengths must match")
    mean = 0.0
    for i, (a, b) in enumerate(zip(y, yhat), 1):
        error = abs(a - b)
        if not math.isfinite(error):
            raise ValueError("residual overflow")
        value = 0.5 * error * error if error <= d else d * (error - 0.5 * d)
        if not math.isfinite(value):
            raise ValueError("loss overflow")
        mean += (value - mean) / i
    if not math.isfinite(mean):
        raise ValueError("nonfinite mean loss")
    return max(0.0, mean)
