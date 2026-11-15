"""Prefix-aware beam search for finite-vocabulary autoregressive decoding."""

import math


def _positive_int(value, name, maximum):
    if isinstance(value, bool) or not isinstance(value, int) or not 1 <= value <= maximum:
        raise ValueError(f"{name} must be an integer between 1 and {maximum}")


def beam_search_decoding(next_log_scores, vocabulary_size, beam_width,
                         max_new_tokens, eos_token_id=None):
    """Return best (zero-based token sequence, cumulative log score) pairs.

    next_log_scores(prefix_tuple) must supply one nonpositive log score per
    token. Negative infinity means masked out. Completed EOS hypotheses are
    preserved without another callback invocation.
    """
    if not callable(next_log_scores):
        raise ValueError("next_log_scores must be callable")
    _positive_int(vocabulary_size, "vocabulary_size", 4096)
    _positive_int(beam_width, "beam_width", 128)
    if isinstance(max_new_tokens, bool) or not isinstance(max_new_tokens, int) or \
            not 0 <= max_new_tokens <= 256:
        raise ValueError("max_new_tokens must be an integer from 0 to 256")
    if eos_token_id is not None and (isinstance(eos_token_id, bool) or
                                     not isinstance(eos_token_id, int) or
                                     not 0 <= eos_token_id < vocabulary_size):
        raise ValueError("eos_token_id must be a valid zero-based token")

    beams = [((), 0.0, False)]
    for _ in range(max_new_tokens):
        candidates = []
        for prefix, cumulative, finished in beams:
            if finished:
                candidates.append((prefix, cumulative, True))
                continue
            values = next_log_scores(prefix)
            if not isinstance(values, (tuple, list)) or len(values) != vocabulary_size:
                raise ValueError("callback must return exactly vocabulary_size scores")
            for token, value in enumerate(values):
                if isinstance(value, bool) or not isinstance(value, (int, float)):
                    raise ValueError("scores must be nonpositive real numbers")
                try:
                    score = float(value)
                except (OverflowError, ValueError):
                    raise ValueError("scores must be nonpositive real numbers") from None
                if math.isnan(score) or score > 0:
                    raise ValueError("scores must be nonpositive real numbers")
                if score == -math.inf:
                    continue  # Masked token.
                next_total = cumulative + score
                if not math.isfinite(next_total):
                    raise ValueError("cumulative log score overflowed")
                candidates.append((prefix + (token,), next_total, token == eos_token_id))
        if not candidates:
            raise ValueError("no valid continuation is available")
        candidates.sort(key=lambda candidate: (-candidate[1], candidate[0]))
        beams = candidates[:beam_width]
        if all(finished for _, _, finished in beams):
            break
    return [(list(prefix), score) for prefix, score, _ in beams]
