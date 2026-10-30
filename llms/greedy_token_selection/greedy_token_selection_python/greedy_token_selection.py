"""Deterministic zero-based argmax token selection for language-model logits."""
import math
from typing import Sequence


def greedy_token_selection(logits: Sequence[float]) -> int:
    """Return the first maximum-logit token ID or raise ValueError."""
    if len(logits) == 0:
        raise ValueError("logits must be nonempty")
    selected = 0
    maximum = -math.inf
    for i, value in enumerate(logits):
        if isinstance(value, bool) or not isinstance(value, (int, float)):
            raise ValueError("logits must contain finite real values")
        try:
            score = float(value)
        except OverflowError as exc:
            raise ValueError("logits must be representable") from exc
        if not math.isfinite(score):
            raise ValueError("logits must be finite")
        if score > maximum:
            maximum = score
            selected = i
    return selected
