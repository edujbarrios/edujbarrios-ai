"""KMP search for every literal occurrence in printable ASCII text."""


def _checked(value: str) -> str:
    if (not isinstance(value, str) or len(value) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in value)):
        raise ValueError("expected a printable ASCII string of at most 512 characters")
    return value


def kmp_substring_search(text: str, pattern: str) -> list[int]:
    """Return all zero-based offsets, including overlaps and empty-pattern boundaries."""
    text, pattern = _checked(text), _checked(pattern)
    m = len(pattern)
    if not m:
        return list(range(len(text) + 1))
    prefix = [0] * m
    j = 0
    for i in range(1, m):
        while j and pattern[i] != pattern[j]:
            j = prefix[j - 1]
        if pattern[i] == pattern[j]:
            j += 1
        prefix[i] = j
    positions = []
    j = 0
    for i, char in enumerate(text):
        while j and char != pattern[j]:
            j = prefix[j - 1]
        if char == pattern[j]:
            j += 1
        if j == m:
            positions.append(i - m + 1)
            j = prefix[j - 1]
    return positions
