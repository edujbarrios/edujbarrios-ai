"""Full-batch gradient descent for a one-feature linear model."""

import math
from typing import Sequence


def gradient_descent(x: Sequence[float], y: Sequence[float],
                     learning_rate: float, epochs: int) -> tuple[float, float]:
    """Return (weight, bias) after exactly the specified number of updates."""
    if len(x) != len(y) or not x:
        raise ValueError("paired nonempty observations required")
    if isinstance(epochs, bool) or not isinstance(epochs, int) or epochs <= 0:
        raise ValueError("epochs must be a positive integer")
    if isinstance(learning_rate, bool) or not isinstance(learning_rate, (float, int)):
        raise ValueError("learning_rate must be positive and finite")
    try:
        lr = float(learning_rate)
    except OverflowError as exc:
        raise ValueError("learning rate is not finite") from exc
    if not math.isfinite(lr) or lr <= 0.0:
        raise ValueError("learning_rate must be positive and finite")
    def checked(values: Sequence[float]) -> list[float]:
        out = []
        for value in values:
            if isinstance(value, bool) or not isinstance(value, (float, int)):
                raise ValueError("observations must be finite real numbers")
            try:
                number = float(value)
            except OverflowError as exc:
                raise ValueError("observations must be finite") from exc
            if not math.isfinite(number):
                raise ValueError("observations must be finite")
            out.append(number)
        return out
    xs, ys = checked(x), checked(y)
    weight = bias = 0.0
    for _ in range(epochs):
        gw = gb = 0.0
        for a, b in zip(xs, ys):
            prediction = weight * a + bias
            error = prediction - b
            component = error * a
            if not all(math.isfinite(z) for z in (prediction, error, component)):
                raise ValueError("non-finite training intermediate")
            gw += component
            gb += error
            if not (math.isfinite(gw) and math.isfinite(gb)):
                raise ValueError("gradient overflow")
        factor = 2.0 * lr / len(xs)
        weight -= factor * gw
        bias -= factor * gb
        if not all(math.isfinite(z) for z in (factor, weight, bias)):
            raise ValueError("training diverged")
    return weight, bias
