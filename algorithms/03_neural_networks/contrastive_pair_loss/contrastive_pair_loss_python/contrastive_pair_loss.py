"""Classic squared-Euclidean contrastive pair loss."""
import math


def contrastive_pair_loss(first: list[float], second: list[float],
                          label: int, margin: float) -> float:
    """Label 1 attracts similar pairs; label 0 repels within the margin."""
    if type(label) is not int or label not in (0, 1):
        raise ValueError("binary integer label required")
    if isinstance(margin, bool) or not isinstance(margin, (int, float)) or \
            not math.isfinite(margin) or not 0 <= margin <= 1000:
        raise ValueError("margin must be finite and in [0,1000]")
    if not isinstance(first, (list, tuple)) or not 1 <= len(first) <= 512 or \
            not isinstance(second, (list, tuple)) or len(first) != len(second):
        raise ValueError("two same-length 1..512 dimensional vectors required")
    for value in (*first, *second):
        if isinstance(value, bool) or not isinstance(value, (int, float)) or \
                not math.isfinite(value) or abs(value) > 1000:
            raise ValueError("finite coordinate in [-1000,1000] required")
    squared = sum((a - b) ** 2 for a, b in zip(first, second))
    gap = max(0.0, margin - math.sqrt(squared))
    return 0.5 * (squared if label == 1 else gap * gap)
