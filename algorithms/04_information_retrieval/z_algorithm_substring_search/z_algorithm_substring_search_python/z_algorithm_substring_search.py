"""Linear-time exact ASCII substring search using the Z-algorithm."""


def _checked(value: str) -> str:
    if (not isinstance(value, str) or len(value) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in value)):
        raise ValueError("expected printable ASCII string <=512 characters")
    return value


def z_algorithm_substring_search(text: str, pattern: str) -> list[int]:
    """Return all zero-based substring occurrences including overlaps."""
    text, pattern = _checked(text), _checked(pattern)
    m = len(pattern)
    if m == 0:
        return list(range(len(text) + 1))
    if m > len(text):
        return []
    seq = [ord(ch) for ch in pattern] + [0] + [ord(ch) for ch in text]
    n = len(seq)
    z = [0] * n
    left = right = 0
    matches = []
    for i in range(1, n):
        if i < right:
            z[i] = min(right - i, z[i - left])
        while i + z[i] < n and seq[z[i]] == seq[i + z[i]]:
            z[i] += 1
        if i + z[i] > right:
            left, right = i, i + z[i]
        if i >= m + 1 and z[i] >= m:
            matches.append(i - m - 1)
    return matches
