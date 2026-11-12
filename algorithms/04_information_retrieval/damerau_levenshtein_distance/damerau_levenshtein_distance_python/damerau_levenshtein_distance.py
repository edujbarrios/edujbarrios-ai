"""Unrestricted Damerau-Levenshtein distance for byte token sequences."""


def damerau_levenshtein_distance(first: list[int], second: list[int]) -> int:
    """Return minimum insert/delete/substitute/adjacent-swap operations."""
    def validate(tokens: list[int]) -> list[int]:
        if not isinstance(tokens, (list, tuple)) or len(tokens) > 256:
            raise ValueError("a byte token list or tuple of length <= 256 is required")
        if any(isinstance(x, bool) or not isinstance(x, int) or
               not 0 <= x <= 255 for x in tokens):
            raise ValueError("byte token IDs must be integers from 0 to 255")
        return tokens

    a, b = validate(first), validate(second)
    n, m = len(a), len(b)
    upper = n + m
    h = [[0] * (m + 2) for _ in range(n + 2)]
    h[0][0] = upper
    for i in range(n + 1):
        h[i + 1][0] = upper
        h[i + 1][1] = i
    for j in range(m + 1):
        h[0][j + 1] = upper
        h[1][j + 1] = j

    seen = [0] * 256
    for i in range(1, n + 1):
        last_match = 0
        for j in range(1, m + 1):
            prev_a, prev_b = seen[b[j - 1]], last_match
            cost = int(a[i - 1] != b[j - 1])
            if cost == 0:
                last_match = j
            h[i + 1][j + 1] = min(
                h[i][j] + cost,
                h[i + 1][j] + 1,
                h[i][j + 1] + 1,
                h[prev_a][prev_b] + (i - prev_a - 1) + 1 + (j - prev_b - 1),
            )
        seen[a[i - 1]] = i
    return h[n + 1][m + 1]
