"""Exact overlap coefficient on signed 32-bit integer token sets."""


def overlap_coefficient(first: list[int], second: list[int]) -> float:
    """Return intersection size / smaller set size (both empty -> 1)."""
    def check(values: list[int]) -> list[int]:
        if not isinstance(values, (list, tuple)):
            raise ValueError("integer sequence required")
        for item in values:
            if isinstance(item, bool) or not isinstance(item, int) or \
                    not -(2**31) <= item <= 2**31 - 1:
                raise ValueError("signed 32-bit integer IDs required")
        return sorted(set(values))

    a, b = check(first), check(second)
    if not a and not b:
        return 1.0
    if not a or not b:
        return 0.0
    i = j = common = 0
    while i < len(a) and j < len(b):
        if a[i] == b[j]:
            common += 1
            i += 1
            j += 1
        elif a[i] < b[j]:
            i += 1
        else:
            j += 1
    return common / min(len(a), len(b))
