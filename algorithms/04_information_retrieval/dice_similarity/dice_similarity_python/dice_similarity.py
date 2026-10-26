"""Exact Dice similarity for sets of signed 32-bit integer token IDs."""


MIN_ID, MAX_ID = -(2**31), 2**31 - 1


def dice_similarity(first: list[int], second: list[int]) -> float:
    """Treat sequences as sets; return 1.0 for two empty sets."""
    def validate(ids: list[int]) -> list[int]:
        if not isinstance(ids, (list, tuple)):
            raise ValueError("input must be an integer sequence")
        for token in ids:
            if isinstance(token, bool) or not isinstance(token, int):
                raise ValueError("token IDs must be integers")
            if token < MIN_ID or token > MAX_ID:
                raise ValueError("token ID outside signed 32-bit range")
        return sorted(set(ids))

    a, b = validate(first), validate(second)
    if not a and not b:
        return 1.0
    i = j = overlap = 0
    while i < len(a) and j < len(b):
        if a[i] == b[j]:
            overlap += 1
            i += 1
            j += 1
        elif a[i] < b[j]:
            i += 1
        else:
            j += 1
    return 2.0 * overlap / (len(a) + len(b))
