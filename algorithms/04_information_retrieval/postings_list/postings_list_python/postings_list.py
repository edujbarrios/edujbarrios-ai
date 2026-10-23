"""Build a one-term inverted postings list from integer-token documents."""
from typing import Sequence

MIN_ID, MAX_ID = -(2 ** 31), 2 ** 31 - 1


def _id(value: int) -> int:
    if isinstance(value, bool) or not isinstance(value, int) or not MIN_ID <= value <= MAX_ID:
        raise ValueError("token IDs must be signed 32-bit integers")
    return value


def postings_list(documents: Sequence[Sequence[int]], target: int) -> list[tuple[int, int]]:
    """Return (zero-based doc ID, term count) for each matching document."""
    _id(target)
    if not isinstance(documents, (list, tuple)):
        raise ValueError("documents must be a sequence")
    result = []
    for doc_id, document in enumerate(documents):
        if not isinstance(document, (list, tuple)):
            raise ValueError("every document must be a sequence")
        frequency = 0
        for token in document:
            if _id(token) == target:
                frequency += 1
        if frequency > 0:
            result.append((doc_id, frequency))
    return result
