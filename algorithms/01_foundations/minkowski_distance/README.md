# Minkowski Distance

## Definition

For nonempty matching vectors x, y and finite p >= 1:

    L_p(x,y) = (sum_i |x_i-y_i|^p)^(1/p)

p=1 is Manhattan distance; p=2 is Euclidean distance. For
numerical stability, let s=max_i |x_i-y_i| and compute
s*(sum_i (|x_i-y_i|/s)^p)^(1/p); return zero if s=0.

## Contract

- Nonempty equally long finite real vectors and finite p >= 1.
- Invalid, nonfinite, mismatched or unrepresentable results are rejected.
- Python rejects nonnumeric and Boolean items with ValueError; R rejects
  complex, matrix and nonnumeric inputs; Rust returns Err; C returns
  false without writing to the output. C needs valid n-element arrays
  and a writable nonnull result pointer.
- This is finite-p only: Chebyshev distance handles p=infinity.

## Complexity

O(n) time; O(1) auxiliary space excluding Python's temporary arrays.

## Implementations

| Language | Folder |
| --- | --- |
| C | [minkowski_distance_c](minkowski_distance_c/) |
| Python | [minkowski_distance_python](minkowski_distance_python/) |
| Rust | [minkowski_distance_rust](minkowski_distance_rust/) |
| R | [minkowski_distance_r](minkowski_distance_r/) |

## Examples

    L_1([0,0], [3,4]) = 7
    L_2([0,0], [3,4]) = 5
    L_3([0,0], [3,4]) = 91^(1/3)

From the repository root, run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

A difference of two finite, opposite-sign near-maximum doubles can
overflow, and is rejected. This CPU reference does not compute gradients
or nearest-neighbor search itself.

## Reference

Minkowski, H. (1910). *Geometrie der Zahlen*. Teubner.
