"""Boyer-Moore-Horspool exact printable ASCII substring search."""


def _checked(s: str) -> str:
    if (not isinstance(s, str) or len(s) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in s)):
        raise ValueError("expected printable ASCII string <=512 characters")
    return s


def boyer_moore_horspool_search(text: str, pattern: str) -> list[int]:
    """All zero-based matches, including overlapping occurrences."""
    text, pattern = _checked(text), _checked(pattern)
    n, m = len(text), len(pattern)
    if m == 0:
        return list(range(n + 1))
    if m > n:
        return []
    shift = [m] * 128
    for j in range(m - 1):
        shift[ord(pattern[j])] = m - 1 - j
    result = []
    start = 0
    while start <= n - m:
        j = m - 1
        while j >= 0 and text[start + j] == pattern[j]:
            j -= 1
        if j < 0:
            result.append(start)
        start += shift[ord(text[start + m - 1])]
    return result
