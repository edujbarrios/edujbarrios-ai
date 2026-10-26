# Canberra Distance

## Why it belongs in this repository

Canberra distance compares numerical feature vectors by weighting coordinate
differences relative to their magnitudes. It can be useful for feature-space
comparisons when relative differences matter more than absolute differences.
Unlike Euclidean distance, values near zero can strongly affect the score.

## Mathematical definition

For equally sized vectors x and y:

    d(x, y) = sum_i |x_i - y_i| / (|x_i| + |y_i|)

The term for x_i = y_i = 0 is defined as zero. To avoid overflowing the
numerator or denominator for large finite inputs, each nonzero term is
computed after dividing both coordinates by
max(|x_i|, |y_i|).

## Contract

- Inputs: equally sized, **nonempty**, one-dimensional vectors of finite
  real numbers; positive and negative values are permitted.
- Output: a nonnegative distance at most n, where n is vector length.
- Both zero coordinates contribute zero. A zero and nonzero coordinate
  contribute one. Identical inputs return zero.
- Python raises ValueError for mismatched/empty vectors, booleans, nonnumeric
  or nonfinite inputs. R rejects nonnumeric/complex/matrix/nonfinite inputs.
  Rust returns Err; C returns false and leaves the result unchanged.
- In C, both input pointers must address n doubles and output must be
  writable; no null pointers are allowed.

## Complexity

O(n) time and O(1) auxiliary storage, excluding the returned scalar.

## Implementations

| Language | Folder |
| --- | --- |
| C | [canberra_distance_c](canberra_distance_c/) |
| Python | [canberra_distance_python](canberra_distance_python/) |
| Rust | [canberra_distance_rust](canberra_distance_rust/) |
| R | [canberra_distance_r](canberra_distance_r/) |

## Examples and tests

    d([1, 2], [2, 4])     = 2/3
    d([0, 0], [0, 0])     = 0
    d([1, -1], [-1, 1])  = 2

From the repository root, run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Canberra distance is sensitive to small values near zero, and unstandardized
feature dimensions may have unexpected effects. This is a clear CPU reference
implementation, not a vectorized library kernel; rounding may vary by
language. Empty vectors are rejected by contract.

## References

SciPy community. (n.d.). *scipy.spatial.distance.canberra*.
SciPy documentation.
https://docs.scipy.org/doc/scipy/reference/generated/scipy.spatial.distance.canberra.html
