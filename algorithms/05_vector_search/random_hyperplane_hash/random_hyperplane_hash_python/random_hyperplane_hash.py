"""Sign hashing of finite vectors with caller-supplied hyperplane normals."""
import math
from typing import Sequence

def _number(value: float) -> float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError("components must be real numbers")
    try:
        result = float(value)
    except OverflowError as exc:
        raise ValueError("components must be representable") from exc
    if not math.isfinite(result):
        raise ValueError("components must be finite")
    return result

def random_hyperplane_hash(x: Sequence[float],
                           normals: Sequence[Sequence[float]]) -> int:
    """Pack p sign bits (first normal is least significant)."""
    if not isinstance(x, (list,tuple)) or len(x) == 0:
        raise ValueError("nonempty feature vector required")
    vector = [_number(v) for v in x]
    if not any(v != 0.0 for v in vector):
        raise ValueError("zero query vector")
    if not isinstance(normals, (list,tuple)) or not 1 <= len(normals) <= 30:
        raise ValueError("1 to 30 normals required")
    signature = 0
    for j, row in enumerate(normals):
        if not isinstance(row, (list,tuple)) or len(row) != len(vector):
            raise ValueError("normal dimension mismatch")
        normal = [_number(v) for v in row]
        if not any(v != 0.0 for v in normal):
            raise ValueError("zero hyperplane normal")
        total = 0.0
        for a, b in zip(vector, normal):
            product = a * b
            total += product
            if not math.isfinite(product) or not math.isfinite(total):
                raise ValueError("dot product overflow")
        if total >= 0.0:
            signature |= 1 << j
    return signature
