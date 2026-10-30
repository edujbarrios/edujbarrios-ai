"""One-time logit penalty for tokens already present in the history."""
import math
from typing import Sequence

def token_presence_penalty(logits: Sequence[float], history: Sequence[int],
                           penalty: float) -> list[float]:
    """Return adjusted logits; raise ValueError on invalid inputs."""
    if isinstance(penalty, bool) or not isinstance(penalty, (int, float)):
        raise ValueError("penalty must be numeric")
    try:
        alpha = float(penalty)
    except OverflowError as exc:
        raise ValueError("penalty must be representable") from exc
    if not math.isfinite(alpha) or not 0 <= alpha <= 5:
        raise ValueError("penalty must be finite in [0, 5]")
    if not 1 <= len(logits) <= 4096 or len(history) > 4096:
        raise ValueError("invalid logits/history length")
    values = []
    for value in logits:
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("logits must be finite numeric values")
        try:
            score = float(value)
        except OverflowError as exc:
            raise ValueError("logits must be representable") from exc
        if not math.isfinite(score) or not -1000 <= score <= 1000:
            raise ValueError("logits outside supported range")
        values.append(score)
    present = [False] * len(values)
    for token in history:
        if type(token) is not int or not 0 <= token < len(values):
            raise ValueError("history IDs must be zero-based integers in vocabulary")
        present[token] = True
    return [value - alpha if present[i] else value for i, value in enumerate(values)]
