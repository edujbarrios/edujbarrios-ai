"""One deterministic AdaGrad step with explicit squared-gradient state."""
import math


def _real(value):
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError("finite real number required")
    try:
        number = float(value)
    except (OverflowError, ValueError) as exc:
        raise ValueError("unrepresentable real number") from exc
    if not math.isfinite(number):
        raise ValueError("finite real number required")
    return number


def adagrad_optimizer(parameters, gradients, accumulator, learning_rate, epsilon):
    """Return (updated_parameters, updated_accumulator)."""
    lr, eps = _real(learning_rate), _real(epsilon)
    if lr <= 0.0 or eps <= 0.0:
        raise ValueError("learning rate and epsilon must be positive")
    vectors = (parameters, gradients, accumulator)
    if any(not isinstance(v, (list, tuple)) for v in vectors):
        raise ValueError("expected three aligned vectors")
    if not (len(parameters) == len(gradients) == len(accumulator)):
        raise ValueError("length mismatch")
    ws, gs, hs = [[_real(x) for x in v] for v in vectors]
    if any(value < 0.0 for value in hs):
        raise ValueError("accumulators must be nonnegative")
    next_w, next_h = [], []
    for w, g, h in zip(ws, gs, hs):
        updated_h = h + g * g
        denominator = math.sqrt(updated_h) + eps
        step = lr * g / denominator
        updated_w = w - step
        if not all(math.isfinite(v) for v in (updated_h, denominator, step, updated_w)):
            raise ValueError("nonfinite AdaGrad intermediate")
        next_w.append(updated_w)
        next_h.append(updated_h)
    return next_w, next_h
