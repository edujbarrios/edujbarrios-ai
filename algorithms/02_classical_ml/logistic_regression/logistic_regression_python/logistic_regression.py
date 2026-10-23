"""Full-batch binary logistic regression for a single numeric predictor."""
import math
from typing import Sequence


def _checked(values: Sequence[float]) -> list[float]:
    converted = []
    for value in values:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("observations must be finite real numbers")
        try:
            number = float(value)
        except OverflowError as exc:
            raise ValueError("observations must be representable") from exc
        if not math.isfinite(number):
            raise ValueError("observations must be finite")
        converted.append(number)
    return converted


def _sigmoid(z: float) -> float:
    if z >= 0.0:
        e = math.exp(-z)
        return 1.0 / (1.0 + e)
    e = math.exp(z)
    return e / (1.0 + e)


def logistic_regression(x: Sequence[float], y: Sequence[float],
                        learning_rate: float, epochs: int) -> tuple[float, float]:
    """Return (weight, bias) after exactly epochs full-batch updates."""
    if len(x) != len(y) or not x:
        raise ValueError("paired nonempty inputs required")
    if isinstance(epochs, bool) or not isinstance(epochs, int) or epochs <= 0:
        raise ValueError("epochs must be a positive integer")
    if isinstance(learning_rate, bool) or not isinstance(learning_rate, (int, float)):
        raise ValueError("learning_rate must be finite and positive")
    try:
        lr = float(learning_rate)
    except OverflowError as exc:
        raise ValueError("learning_rate must be representable") from exc
    if not math.isfinite(lr) or lr <= 0.0:
        raise ValueError("learning_rate must be finite and positive")
    xs, ys = _checked(x), _checked(y)
    if any(label not in (0.0, 1.0) for label in ys):
        raise ValueError("labels must be exactly 0 or 1")

    weight = bias = 0.0
    for _ in range(epochs):
        grad_weight = grad_bias = 0.0
        for a, label in zip(xs, ys):
            z = weight * a + bias
            if not math.isfinite(z):
                raise ValueError("non-finite logit")
            error = _sigmoid(z) - label
            component = error * a
            if not math.isfinite(component):
                raise ValueError("gradient overflow")
            grad_weight += component
            grad_bias += error
            if not (math.isfinite(grad_weight) and math.isfinite(grad_bias)):
                raise ValueError("gradient overflow")
        factor = lr / len(xs)
        dw, db = factor * grad_weight, factor * grad_bias
        if not all(map(math.isfinite, (factor, dw, db))):
            raise ValueError("parameter update overflow")
        weight -= dw
        bias -= db
        if not math.isfinite(weight) or not math.isfinite(bias):
            raise ValueError("training diverged")
    return weight, bias
