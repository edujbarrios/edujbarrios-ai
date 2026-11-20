"""Inverted dropout forward transform with an explicit binary mask."""
import math


def _finite_real(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError("finite real number required")
    try:
        number = float(value)
    except (OverflowError, ValueError) as exc:
        raise ValueError("unrepresentable real number") from exc
    if not math.isfinite(number):
        raise ValueError("finite real number required")
    return number


def inverted_dropout(activations: list[float], mask: list[int],
                     probability: float) -> list[float]:
    """Return scaled retained activations and zeroed masked activations."""
    p = _finite_real(probability)
    if not 0.0 <= p < 1.0:
        raise ValueError("dropout probability must be in [0, 1)")
    if (not isinstance(activations, (list, tuple)) or
            not isinstance(mask, (list, tuple)) or
            len(activations) != len(mask)):
        raise ValueError("aligned activation and binary mask vectors required")
    values = [_finite_real(value) for value in activations]
    if any(isinstance(bit, bool) or not isinstance(bit, int) or bit not in (0, 1)
           for bit in mask):
        raise ValueError("mask must contain binary integers")
    keep = 1.0 - p
    result = []
    for value, bit in zip(values, mask):
        scaled = value / keep if bit else 0.0
        if not math.isfinite(scaled):
            raise ValueError("retained activation scaling overflow")
        result.append(scaled)
    return result
