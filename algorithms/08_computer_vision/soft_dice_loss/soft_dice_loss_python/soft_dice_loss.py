"""Smoothed soft Dice loss for probabilistic binary segmentation."""


def soft_dice_loss(predictions: list[float], targets: list[int]) -> float:
    """Return 1 - (2 overlap + 1)/(sum predictions + sum labels + 1)."""
    import math

    if not isinstance(predictions, (list, tuple)) or not 1 <= len(predictions) <= 512 or \
            not isinstance(targets, (list, tuple)) or \
            len(targets) != len(predictions):
        raise ValueError("aligned 1..512 element sequences required")
    for p, t in zip(predictions, targets):
        if isinstance(p, bool) or not isinstance(p, (int, float)) or \
                not math.isfinite(p) or not 0 <= p <= 1:
            raise ValueError("finite probabilities in [0,1] required")
        if type(t) is not int or t not in (0, 1):
            raise ValueError("integer binary targets required")

    intersection = sum(p * t for p, t in zip(predictions, targets))
    return 1.0 - (2 * intersection + 1.0) / (
        sum(predictions) + sum(targets) + 1.0)
