"""Two-pointer AND merge of strictly increasing document-ID lists."""
from typing import Sequence

MAX_ID = 2**31 - 1


def _validate(values: Sequence[int]) -> None:
    previous = -1
    for value in values:
        if isinstance(value, bool) or not isinstance(value, int) or not 0 <= value <= MAX_ID:
            raise ValueError("document IDs must be integers in 0..2^31-1")
        if value <= previous:
            raise ValueError("document IDs must be strictly increasing")
        previous = value


def postings_intersection(left: Sequence[int], right: Sequence[int]) -> list[int]:
    """Return ascending unique document IDs present in both inputs."""
    _validate(left)
    _validate(right)
    i = j = 0
    result = []
    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            i += 1
        elif left[i] > right[j]:
            j += 1
        else:
            result.append(left[i])
            i += 1
            j += 1
    return result
