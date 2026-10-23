"""Dense affine neural layer followed by ReLU, implemented from scratch."""
import math
from typing import Sequence


def _real(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError("components must be real numbers")
    try:
        number = float(value)
    except OverflowError as exc:
        raise ValueError("components must be finite") from exc
    if not math.isfinite(number):
        raise ValueError("components must be finite")
    return number


def dense_forward(input: Sequence[float],
                  weights: Sequence[Sequence[float]],
                  bias: Sequence[float]) -> list[float]:
    """Return relu(W @ input + bias), with output neurons in matrix rows."""
    if not isinstance(input, (list, tuple)) or not input:
        raise ValueError("input must be a nonempty vector")
    x = [_real(value) for value in input]
    if not isinstance(weights, (list, tuple)) or not weights:
        raise ValueError("weights must contain at least one row")
    if not isinstance(bias, (list, tuple)) or len(bias) != len(weights):
        raise ValueError("one bias required per output neuron")
    matrix = []
    for row in weights:
        if not isinstance(row, (list, tuple)) or len(row) != len(x):
            raise ValueError("weight matrix shape does not match input")
        matrix.append([_real(value) for value in row])
    biases = [_real(value) for value in bias]

    output = []
    for row, offset in zip(matrix, biases):
        total = 0.0
        for weight, value in zip(row, x):
            product = weight * value
            total += product
            if not math.isfinite(product) or not math.isfinite(total):
                raise ValueError("affine multiplication overflow")
        z = total + offset
        if not math.isfinite(z):
            raise ValueError("affine addition overflow")
        output.append(max(0.0, z))
    return output
