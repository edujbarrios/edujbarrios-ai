"""Greedy best-path CTC decoding; collapse repeated tokens before blanks."""

import math


def ctc_greedy_decoding(probabilities, blank_id=0):
    """Return decoded tokens, frame alignment and best-path log probability."""
    if not isinstance(probabilities, (list, tuple)) or not 1 <= len(probabilities) <= 256:
        raise ValueError("probabilities must have 1..256 time steps")
    first = probabilities[0]
    if not isinstance(first, (tuple, list)) or not 2 <= len(first) <= 128:
        raise ValueError("vocabulary size must be 2..128")
    size = len(first)
    if (isinstance(blank_id, bool) or not isinstance(blank_id, int)
            or not 0 <= blank_id < size):
        raise ValueError("blank_id must be a valid zero-based token ID")

    alignment = []
    log_probability = 0.0
    for row in probabilities:
        if not isinstance(row, (tuple, list)) or len(row) != size:
            raise ValueError("all probability rows must have the same size")
        values = []
        for probability in row:
            if isinstance(probability, bool) or not isinstance(probability, (int, float)):
                raise ValueError("probabilities must be finite reals in [0,1]")
            try:
                number = float(probability)
            except (ValueError, OverflowError):
                raise ValueError("probabilities must be finite reals in [0,1]") from None
            if not math.isfinite(number) or not 0 <= number <= 1:
                raise ValueError("probabilities must be finite reals in [0,1]")
            values.append(number)
        if abs(math.fsum(values) - 1.0) > 1e-9:
            raise ValueError("each probability row must sum to one")
        selected = max(range(size), key=lambda i: values[i])
        alignment.append(selected)
        log_probability += math.log(values[selected])

    decoded = []
    previous = None
    for selected in alignment:
        if selected != previous and selected != blank_id:
            decoded.append(selected)
        previous = selected
    return {"token_ids": decoded, "alignment": alignment,
            "path_log_probability": log_probability}
