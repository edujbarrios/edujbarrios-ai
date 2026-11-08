"""Longest contiguous common ASCII substring, with deterministic tie-breaking."""


def _checked(value: str) -> str:
    if (not isinstance(value, str) or len(value) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in value)):
        raise ValueError("expected printable ASCII of length at most 512")
    return value


def longest_common_substring(a: str, b: str) -> tuple[int, int, int]:
    """Return (length, a_offset, b_offset), choosing earliest a then b ties."""
    a, b = _checked(a), _checked(b)
    prev = [0] * (len(b) + 1)
    best, start_a, start_b = 0, 0, 0
    for i, left in enumerate(a, 1):
        curr = [0] * (len(b) + 1)
        for j, right in enumerate(b, 1):
            if left != right:
                continue
            length = prev[j - 1] + 1
            curr[j] = length
            sa, sb = i - length, j - length
            if (length > best or
                    (length == best and length > 0
                     and (sa, sb) < (start_a, start_b))):
                best, start_a, start_b = length, sa, sb
        prev = curr
    return best, start_a, start_b
