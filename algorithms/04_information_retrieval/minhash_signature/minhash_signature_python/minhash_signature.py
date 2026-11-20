"""Deterministic MinHash-style signature using fixed modular linear hashes."""

PRIME = 65521


def _validated(values, minimum: int, maximum: int, name: str) -> list[int]:
    if not isinstance(values, (list, tuple)):
        raise ValueError(f"{name} must be an integer sequence")
    for x in values:
        if isinstance(x, bool) or not isinstance(x, int) or not minimum <= x <= maximum:
            raise ValueError(f"{name} values outside allowed integer range")
    return list(values)


def minhash_signature(tokens, a, b) -> list[int]:
    """Return one minimum modular hash per supplied coefficient pair."""
    ids = _validated(tokens, 0, PRIME - 1, "tokens")
    multipliers = _validated(a, 1, PRIME - 1, "a")
    offsets = _validated(b, 0, PRIME - 1, "b")
    if not multipliers or len(multipliers) != len(offsets):
        raise ValueError("one or more aligned hash coefficient pairs required")
    signature = [PRIME] * len(multipliers)
    for x in ids:
        for j, (multiplier, offset) in enumerate(zip(multipliers, offsets)):
            hashed = (multiplier * x + offset) % PRIME
            signature[j] = min(signature[j], hashed)
    return signature
