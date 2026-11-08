"""Jaccard similarity of unique consecutive printable-ASCII character pairs."""


def _checked(value: str) -> str:
    if (not isinstance(value, str) or len(value) > 512
            or any(not 32 <= ord(ch) <= 126 for ch in value)):
        raise ValueError("expected printable ASCII string of length at most 512")
    return value


def character_bigram_jaccard(a: str, b: str) -> float:
    """Compare bigram sets; empty union is defined as similarity 1.0."""
    a, b = _checked(a), _checked(b)
    left = {a[i:i + 2] for i in range(len(a) - 1)}
    right = {b[i:i + 2] for i in range(len(b) - 1)}
    union_size = len(left | right)
    if union_size == 0:
        return 1.0
    return len(left & right) / union_size
