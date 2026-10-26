# Chebyshev (L∞) Distance

## Motivation

Chebyshev distance measures the **largest absolute difference** between corresponding coordinates. It is useful when the worst individual discrepancy matters more than the total of all differences, for example, coordinate-wise tolerance checks in vector search.

## Mathematical definition

For equally sized vectors `a` and `b` with `n` coordinates:

```text
d_inf(a, b) = max(|a[i] - b[i]|), for i = 1,...,n
a = [1, 2, -1]; b = [4, -2, 1]
differences = [3, 4, 2]; d_inf = 4
```

For `n=0` this implementation defines the result as `0` (a programming convention for empty vectors).

## Shared input/output contract

- Input vectors have the same dimension and contain **finite real numbers** only. Python rejects booleans and nonnumeric items; R requires real numeric vectors.
- Reject any non-finite input or subtraction overflow; never silently return an infinite distance.
- C takes two arrays of at least `n` doubles and a nonnull result pointer. Null arrays are permitted only when `n=0`. The output remains unchanged on error; inputs and output must not alias.
- Rust returns `Result<f64, &'static str>`; Python and R raise errors. Successful results are nonnegative finite double-precision scalars.

## Complexity

**O(n)** time. **O(1)** additional space in C, Python and Rust; vectorized R allocates **O(n)** temporary space.

## Implementations

| Language | Files |
| --- | --- |
| C | [chebyshev_distance_c](chebyshev_distance_c/) |
| Python | [chebyshev_distance_python](chebyshev_distance_python/) |
| Rust | [chebyshev_distance_rust](chebyshev_distance_rust/) |
| R | [chebyshev_distance_r](chebyshev_distance_r/) |

Run from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include the example, symmetry, identical and empty vectors, invalid coordinates, mismatched dimensions and overflow.

## Limitations

This unweighted metric is sensitive to the coordinate with the largest absolute deviation. Unscaled features can dominate. No approximate index or CUDA variant is provided.

## References

SciPy community. (n.d.). *scipy.spatial.distance.chebyshev*. SciPy documentation. https://docs.scipy.org/doc/scipy/reference/generated/scipy.spatial.distance.chebyshev.html
