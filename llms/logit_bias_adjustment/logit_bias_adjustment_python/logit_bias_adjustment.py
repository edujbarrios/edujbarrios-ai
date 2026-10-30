"""Sparse additive adjustment of selected next-token logits."""
import math
from typing import Sequence


def logit_bias_adjustment(logits: Sequence[float], token_ids: Sequence[int],
                          biases: Sequence[float]) -> list[float]:
    """Return adjusted logits; reject invalid/duplicate token IDs with ValueError."""
    n = len(logits)
    b = len(token_ids)
    if not 1 <= n <= 4096 or b != len(biases) or b > n:
        raise ValueError("invalid vocabulary size or sparse bias lengths")
    result = []
    for raw in logits:
        if isinstance(raw, bool) or not isinstance(raw, (int, float)):
            raise ValueError("logits must be finite real numbers")
        try:
            score = float(raw)
        except OverflowError as exc:
            raise ValueError("unrepresentable logit") from exc
        if not math.isfinite(score) or abs(score) > 1000:
            raise ValueError("logits must be finite and within [-1000,1000]")
        result.append(score)
    seen = [False] * n
    updates = []
    for token, raw_bias in zip(token_ids, biases):
        if type(token) is not int or not 0 <= token < n or seen[token]:
            raise ValueError("IDs must be distinct zero-based vocabulary integers")
        seen[token] = True
        if isinstance(raw_bias, bool) or not isinstance(raw_bias, (int, float)):
            raise ValueError("biases must be finite real numbers")
        try:
            bias = float(raw_bias)
        except OverflowError as exc:
            raise ValueError("unrepresentable bias") from exc
        if not math.isfinite(bias) or abs(bias) > 100:
            raise ValueError("biases must be finite and within [-100,100]")
        updates.append((token, bias))
    for token, bias in updates:
        result[token] += bias
    return result
