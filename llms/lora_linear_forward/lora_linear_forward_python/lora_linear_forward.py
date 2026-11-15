"""Frozen dense linear map with a scaled LoRA low-rank adapter."""

import math


def _bounded_real(value, name):
    if isinstance(value, bool) or not isinstance(value, (float, int)):
        raise ValueError(f"{name} must contain finite real numbers")
    try:
        number = float(value)
    except (ValueError, OverflowError):
        raise ValueError(f"{name} must contain finite real numbers") from None
    if not math.isfinite(number) or abs(number) > 1000:
        raise ValueError(f"{name} values must have magnitude <= 1000")
    return number


def _vector(values, name, expected=None):
    if not isinstance(values, (tuple, list)) or not 1 <= len(values) <= 128:
        raise ValueError(f"{name} must contain 1..128 elements")
    if expected is not None and len(values) != expected:
        raise ValueError(f"{name} has incompatible dimensions")
    return [_bounded_real(value, name) for value in values]


def _matrix(rows, name, columns):
    if not isinstance(rows, (tuple, list)) or not 1 <= len(rows) <= 128:
        raise ValueError(f"{name} must contain 1..128 rows")
    return [_vector(row, name, columns) for row in rows]


def lora_linear_forward(x, weight, a, b, alpha=1.0, bias=None):
    """Compute W @ x + (alpha/r) * B @ (A @ x) + optional bias.

    W has shape (out, in), A is (rank, in), B is (out, rank).
    """
    features = _vector(x, "x")
    dense = _matrix(weight, "weight", len(features))
    low_a = _matrix(a, "a", len(features))
    low_b = _matrix(b, "b", len(low_a))
    if len(low_b) != len(dense):
        raise ValueError("b rows must match weight rows")
    offset = ([0.0] * len(dense) if bias is None
              else _vector(bias, "bias", len(dense)))
    scale = _bounded_real(alpha, "alpha")
    if scale <= 0:
        raise ValueError("alpha must be strictly positive")
    scale /= len(low_a)

    hidden = [sum(value * feature for value, feature in zip(row, features))
              for row in low_a]
    output = []
    for weight_row, adapter_row, intercept in zip(dense, low_b, offset):
        base = sum(value * feature for value, feature in zip(weight_row, features))
        adapter = sum(value * latent for value, latent in zip(adapter_row, hidden))
        value = base + scale * adapter + intercept
        if not math.isfinite(value):
            raise ValueError("nonfinite intermediate or output")
        output.append(value)
    return output
