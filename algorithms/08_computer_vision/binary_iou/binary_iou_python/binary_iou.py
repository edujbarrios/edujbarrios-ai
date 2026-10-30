"""Intersection over Union for aligned binary segmentation masks."""

from typing import Sequence


def binary_iou(prediction: Sequence[int], target: Sequence[int]) -> float:
    """Return IoU in [0, 1]; raise ValueError for invalid masks."""
    if len(prediction) == 0 or len(prediction) != len(target):
        raise ValueError("masks must be nonempty and have equal length")
    intersection = 0
    union = 0
    for a, b in zip(prediction, target):
        if type(a) is not int or type(b) is not int or a not in (0, 1) or b not in (0, 1):
            raise ValueError("mask pixels must be integers equal to 0 or 1")
        intersection += a & b
        union += a | b
    return intersection / union if union else 1.0
