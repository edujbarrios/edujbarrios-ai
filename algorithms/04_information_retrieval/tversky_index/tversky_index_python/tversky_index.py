"""Tversky set similarity for signed 32-bit token IDs."""
import math


def tversky_index(first, second, alpha=1.0, beta=1.0):
    """Return asymmetric overlap in [0, 1] for positive alpha and beta."""
    def positive(value):
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("strictly positive finite parameters required")
        try:
            real = float(value)
        except OverflowError as exc:
            raise ValueError("unrepresentable parameter") from exc
        if not math.isfinite(real) or real <= 0:
            raise ValueError("strictly positive finite parameters required")
        return real

    def ids(seq):
        if not isinstance(seq, (list, tuple)):
            raise ValueError("token ID sequence required")
        for token in seq:
            if isinstance(token, bool) or not isinstance(token, int) or \
                    not -(2**31) <= token <= 2**31 - 1:
                raise ValueError("signed 32-bit integer token IDs required")
        return sorted(set(seq))

    a_coeff, b_coeff = positive(alpha), positive(beta)
    a, b = ids(first), ids(second)
    if not a and not b:
        return 1.0
    if not a or not b:
        return 0.0
    i = j = intersection = 0
    while i < len(a) and j < len(b):
        if a[i] == b[j]:
            i += 1
            j += 1
            intersection += 1
        elif a[i] < b[j]:
            i += 1
        else:
            j += 1
    denominator = (intersection + a_coeff * (len(a) - intersection) +
                   b_coeff * (len(b) - intersection))
    if not math.isfinite(denominator):
        raise ValueError("unrepresentable denominator")
    return min(1.0, max(0.0, intersection / denominator))
