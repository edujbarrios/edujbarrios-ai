"""Symbol-wise Hamming distance between equal-length byte sequences."""


def hamming_distance(a: bytes | bytearray, b: bytes | bytearray) -> int:
    """Return differing byte positions; raise ValueError on invalid input."""
    if not isinstance(a, (bytes, bytearray)) or not isinstance(b, (bytes, bytearray)):
        raise ValueError("inputs must be bytes or bytearray")
    if len(a) != len(b):
        raise ValueError("inputs must be equal length")
    return sum(left != right for left, right in zip(a, b))
