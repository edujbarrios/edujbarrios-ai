"""Deterministic sinusoidal position embeddings for a Transformer."""
import math

def positional_encoding(length: int, width: int) -> list[list[float]]:
    """Return a length-by-width matrix for zero-based token positions."""
    if (isinstance(length, bool) or not isinstance(length, int) or
        isinstance(width, bool) or not isinstance(width, int) or
        not 1 <= length <= 4096 or not 1 <= width <= 1024):
        raise ValueError("length must be 1..4096, width must be 1..1024")
    result=[]
    for position in range(length):
        row=[]
        for j in range(width):
            pair=j//2
            angle=position/(10000.0**(2.0*pair/width))
            row.append(math.sin(angle) if j%2==0 else math.cos(angle))
        result.append(row)
    return result
