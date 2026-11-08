"""Jaro–Winkler similarity using a fixed 0.7 threshold and 0.1 prefix scale."""


def _checked(value: str) -> str:
    if (not isinstance(value, str) or len(value) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in value)):
        raise ValueError("expected printable ASCII of length at most 512")
    return value


def jaro_winkler_similarity(a: str, b: str) -> float:
    """Case-sensitive Jaro–Winkler score in [0, 1]."""
    a, b = _checked(a), _checked(b)
    n, m = len(a), len(b)
    if n == 0 and m == 0:
        return 1.0
    if n == 0 or m == 0:
        return 0.0

    window = max(0, max(n, m) // 2 - 1)
    matched_a = [False] * n
    matched_b = [False] * m
    matches = 0
    for i, char in enumerate(a):
        for j in range(max(0, i - window), min(m, i + window + 1)):
            if not matched_b[j] and char == b[j]:
                matched_a[i] = matched_b[j] = True
                matches += 1
                break
    if matches == 0:
        return 0.0

    mismatches = 0
    j = 0
    for i, char in enumerate(a):
        if matched_a[i]:
            while not matched_b[j]:
                j += 1
            if char != b[j]:
                mismatches += 1
            j += 1
    transpositions = mismatches / 2.0
    jaro = (matches / n + matches / m + (matches - transpositions) / matches) / 3.0
    prefix = 0
    for i in range(min(4, n, m)):
        if a[i] != b[i]:
            break
        prefix += 1
    return jaro + 0.1 * prefix * (1.0 - jaro) if jaro > 0.7 else jaro
