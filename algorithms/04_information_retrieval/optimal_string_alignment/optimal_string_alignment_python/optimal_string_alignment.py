"""Restricted Damerau (optimal string alignment) edit distance for token IDs."""


def optimal_string_alignment(first: list[int], second: list[int]) -> int:
    """Return OSA distance for two int32 token sequences."""
    def validate(seq: list[int]) -> list[int]:
        if not isinstance(seq, (list, tuple)):
            raise ValueError("integer token sequence required")
        for token in seq:
            if isinstance(token, bool) or not isinstance(token, int) or \
                    token < -(2**31) or token > 2**31 - 1:
                raise ValueError("signed 32-bit integer token ID required")
        return seq

    a, b = validate(first), validate(second)
    if len(a) < len(b):
        a, b = b, a
    width = len(b)
    older = [0] * (width + 1)
    previous = list(range(width + 1))
    current = [0] * (width + 1)
    for i, token in enumerate(a, start=1):
        current[0] = i
        for j, other in enumerate(b, start=1):
            cost = 0 if token == other else 1
            best = min(previous[j] + 1, current[j - 1] + 1,
                       previous[j - 1] + cost)
            if i > 1 and j > 1 and token == b[j - 2] and a[i - 2] == other:
                best = min(best, older[j - 2] + 1)
            current[j] = best
        older, previous, current = previous, current, older
    return previous[width]
