"""Exact Rabin-Karp search of printable ASCII strings."""
BASE = 257
MODULUS = 1_000_000_007


def _checked(value: str) -> str:
    if (not isinstance(value, str) or len(value) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in value)):
        raise ValueError("expected printable ASCII string of at most 512 characters")
    return value


def rabin_karp_substring_search(text: str, pattern: str) -> list[int]:
    """Find every exact zero-based occurrence, including overlaps."""
    text, pattern = _checked(text), _checked(pattern)
    n, m = len(text), len(pattern)
    if m == 0:
        return list(range(n + 1))
    if m > n:
        return []
    high_power = pow(BASE, m - 1, MODULUS)
    pattern_hash = 0
    window_hash = 0
    for i in range(m):
        pattern_hash = (pattern_hash * BASE + ord(pattern[i])) % MODULUS
        window_hash = (window_hash * BASE + ord(text[i])) % MODULUS
    result = []
    for start in range(n - m + 1):
        if pattern_hash == window_hash and text[start:start + m] == pattern:
            result.append(start)
        if start < n - m:
            window_hash = ((window_hash - ord(text[start]) * high_power)
                           * BASE + ord(text[start + m])) % MODULUS
    return result
