# Manhattan (L1) Distance

## Purpose

Manhattan distance computes total absolute displacement across vector coordinates. It can be useful for nearest-neighbor algorithms when deviations should accumulate linearly rather than quadratically.

## Formula and example

```text
L1(a,b) = sum(i=1..n) |a[i] - b[i]|
a = [1, 2, -1]; b = [4, -2, 1]
L1 = 3 + 4 + 2 = 9
```

## Shared contract

- The inputs must have the same dimension; two empty vectors return **0**.
- Every coordinate must be finite. Non-finite inputs, difference overflow and accumulated-sum overflow are errors.
- C receives two arrays and their common length `n` plus an output pointer. Null arrays are allowed only if `n=0`; on error `*out` is unchanged. Arrays and output must not alias.
- Python rejects booleans and non-numeric coordinates. Rust returns `Result<f64, &'static str>`; Python and R raise errors.
- Differences and the final sum use double precision in every implementation.

## Complexity

**O(n)** time; **O(1)** auxiliary space in C, Python and Rust. The vectorized R implementation uses **O(n)** temporary space.

## Implementations

| Language | Implementation |
| --- | --- |
| C | [manhattan_distance_c](manhattan_distance_c/) |
| Python | [manhattan_distance_python](manhattan_distance_python/) |
| Rust | [manhattan_distance_rust](manhattan_distance_rust/) |
| R | [manhattan_distance_r](manhattan_distance_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the example, identical and empty vectors, negative values, invalid types/values, length mismatch and arithmetic overflow.

## Limitations

The metric is unweighted and does not standardize features. Coordinates measured at different scales may dominate. Floating-point summation can lose low-order precision for extreme magnitudes. CUDA is not provided.

## References

SciPy community. (n.d.). *scipy.spatial.distance.cityblock*. SciPy documentation. https://docs.scipy.org/doc/scipy/reference/generated/scipy.spatial.distance.cityblock.html
