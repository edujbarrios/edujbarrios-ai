"""Plain triple-loop dense matrix multiplication."""

import math
from typing import Sequence


def _validate(matrix: Sequence[Sequence[float]]) -> tuple[list[list[float]], int, int]:
    if not matrix or not isinstance(matrix, (list, tuple)):
        raise ValueError("matrix must have at least one row")
    if not isinstance(matrix[0], (list, tuple)) or len(matrix[0]) == 0:
        raise ValueError("matrix must have at least one column")
    columns = len(matrix[0])
    result = []
    for row in matrix:
        if not isinstance(row, (list, tuple)) or len(row) != columns:
            raise ValueError("matrix must be rectangular")
        converted = []
        for value in row:
            if isinstance(value, bool) or not isinstance(value, (int, float)):
                raise ValueError("matrix entries must be real numbers")
            try:
                item = float(value)
            except OverflowError as exc:
                raise ValueError("entry cannot be represented") from exc
            if not math.isfinite(item):
                raise ValueError("matrix entries must be finite")
            converted.append(item)
        result.append(converted)
    return result, len(result), columns


def matrix_multiplication(left: Sequence[Sequence[float]],
                          right: Sequence[Sequence[float]]) -> list[list[float]]:
    """Multiply nonempty rectangular matrices; raise ValueError on failure."""
    a, rows, shared = _validate(left)
    b, right_rows, cols = _validate(right)
    if shared != right_rows:
        raise ValueError("inner matrix dimensions must agree")
    output = [[0.0] * cols for _ in range(rows)]
    for i in range(rows):
        for j in range(cols):
            total = 0.0
            for k in range(shared):
                product = a[i][k] * b[k][j]
                total += product
                if not math.isfinite(product) or not math.isfinite(total):
                    raise ValueError("matrix multiplication overflow")
            output[i][j] = total
    return output
