"""CTC negative log-likelihood via stable forward dynamic programming."""

import math


def _logsumexp(values):
    maximum = max(values)
    if maximum == -math.inf:
        return -math.inf
    return maximum + math.log(math.fsum(
        math.exp(value - maximum) for value in values
    ))


def ctc_loss(probabilities, targets, blank_id=0):
    """Return -log P(targets | frame probabilities); inf if no valid alignment."""
    if (not isinstance(probabilities, (tuple, list)) or
            not 1 <= len(probabilities) <= 256):
        raise ValueError("probabilities must contain 1..256 time steps")
    if (not isinstance(probabilities[0], (tuple, list)) or
            not 2 <= len(probabilities[0]) <= 128):
        raise ValueError("vocabulary size must be 2..128")
    vocabulary_size = len(probabilities[0])
    if (isinstance(blank_id, bool) or not isinstance(blank_id, int)
            or not 0 <= blank_id < vocabulary_size):
        raise ValueError("blank_id must be a valid token ID")
    if not isinstance(targets, (tuple, list)) or len(targets) > 256:
        raise ValueError("targets must contain 0..256 token IDs")
    for token in targets:
        if (isinstance(token, bool) or not isinstance(token, int)
                or not 0 <= token < vocabulary_size or token == blank_id):
            raise ValueError("targets must contain nonblank integer token IDs")

    log_rows = []
    for row in probabilities:
        if not isinstance(row, (tuple, list)) or len(row) != vocabulary_size:
            raise ValueError("all probability rows must have equal shape")
        normalized = []
        for value in row:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("probabilities must be finite numbers in [0,1]")
            try:
                number = float(value)
            except (ValueError, OverflowError):
                raise ValueError("probabilities must be finite numbers in [0,1]") from None
            if not math.isfinite(number) or not 0 <= number <= 1:
                raise ValueError("probabilities must be finite numbers in [0,1]")
            normalized.append(number)
        if abs(math.fsum(normalized) - 1.0) > 1e-9:
            raise ValueError("each probability row must sum to one")
        log_rows.append([math.log(p) if p else -math.inf for p in normalized])

    expanded = [blank_id]
    for token in targets:
        expanded.extend((token, blank_id))
    size = len(expanded)
    previous = [-math.inf] * size
    previous[0] = log_rows[0][blank_id]
    if size > 1:
        previous[1] = log_rows[0][expanded[1]]

    for row in log_rows[1:]:
        current = [-math.inf] * size
        for state, token in enumerate(expanded):
            predecessors = [previous[state]]
            if state > 0:
                predecessors.append(previous[state - 1])
            if (state > 1 and token != blank_id and
                    token != expanded[state - 2]):
                predecessors.append(previous[state - 2])
            incoming = _logsumexp(predecessors)
            if incoming != -math.inf and row[token] != -math.inf:
                current[state] = incoming + row[token]
        previous = current

    final = (_logsumexp(previous[-2:]) if size > 1 else previous[0])
    if final == -math.inf:
        return math.inf
    return max(0.0, -final)  # Guard against tiny negative floating-point noise.
