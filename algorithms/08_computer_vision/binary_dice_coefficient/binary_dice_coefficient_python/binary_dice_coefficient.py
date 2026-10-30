"""Binary foreground Dice score for aligned segmentation masks."""
from typing import Sequence

def binary_dice_coefficient(prediction: Sequence[int], target: Sequence[int]) -> float:
    """Return Dice overlap, rejecting mismatched and non-binary masks."""
    if len(prediction) == 0 or len(prediction) != len(target):
        raise ValueError("masks must be nonempty and equally sized")
    intersection = count_a = count_b = 0
    for a, b in zip(prediction, target):
        if type(a) is not int or type(b) is not int or a not in (0, 1) or b not in (0, 1):
            raise ValueError("mask pixels must be integers 0 or 1")
        count_a += a
        count_b += b
        intersection += a & b
    return 2.0 * intersection / (count_a + count_b) if count_a + count_b else 1.0
