# Euclidean Distance

**Stage:** 01 — Foundations  
**Implementations:** C, Python, Rust and R (CPU)

## Why this algorithm is here

Euclidean (L2) distance is a foundational measure for K-nearest neighbors,
K-means clustering, vector retrieval and geometric reasoning about embeddings.
It naturally follows dot product in the catalogue and precedes exact nearest
neighbor search and vector indexes.

## What it does

Compute the straight-line distance between two equally long real vectors:

    distance(a, b) = sqrt(sum((a[i] - b[i])^2))

Example:

    a = [0, 0]
    b = [3, 4]
    distance(a, b) = 5

## Shared behavior and numerical policy

- Inputs: equally sized sequences of finite real values.
- Output: one nonnegative finite distance.
- Two empty vectors have distance 0.0.
- Equal vectors have distance 0.0.
- Reject NaN, infinity, non-finite differences and non-finite results.
- Python, Rust and R reject mismatched dimensions. C receives the number of
  elements with raw pointers and cannot verify underlying allocation lengths.
- Implementations avoid naive squaring of every difference, which can
  overflow even when the correct Euclidean distance is representable.
  C, Python and Rust use stable `hypot` accumulation; R uses scaled sums
  of squares.
- Input values that produce a difference or final distance outside the
  finite range of 64-bit floating point are rejected.
- Floating-point rounding is platform-dependent. Use numerical tolerances
  for non-exact expected results.

C returns `bool` and writes to the output pointer on success only. Python
raises `ValueError`; Rust returns `Result<f64, &'static str>`; R raises an error.

## Complexity

All four versions perform **O(n)** work. C, Python and Rust need O(1)
extra memory, while R creates an O(n) vector of differences.

## Implementations

| Language | Folder | Dependencies |
| --- | --- | --- |
| C | [euclidean_distance_c/](euclidean_distance_c/) | C11, libm |
| Python | [euclidean_distance_python/](euclidean_distance_python/) | Python 3.10+, standard library |
| Rust | [euclidean_distance_rust/](euclidean_distance_rust/) | Rust 2021 |
| R | [euclidean_distance_r/](euclidean_distance_r/) | Base R |

**No CUDA version yet.** This first PR is deliberately CPU-only so its
numerical behavior is easy to test everywhere. A future CUDA implementation
could be added as a focused PR with GPU tests and transfer-cost benchmarks.

## Test

From the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

Or manually, from this algorithm folder:

    cc -std=c11 euclidean_distance_c/euclidean_distance.c euclidean_distance_c/test_euclidean_distance.c -lm -o /tmp/l2_c && /tmp/l2_c
    python3 -m unittest discover -s euclidean_distance_python -p 'test_*.py' -v
    rustc --edition=2021 --test euclidean_distance_rust/euclidean_distance.rs -o /tmp/l2_rs && /tmp/l2_rs
    Rscript euclidean_distance_r/test_euclidean_distance.R

## Limitations

This is a simple, standalone scalar primitive—not a batch kNN engine,
SIMD implementation or GPU kernel. It does not support broadcasting,
weighted dimensions, missing values or complex numbers. No speed claims.

## References

- [Euclidean distance — Encyclopedia of Mathematics](https://encyclopediaofmath.org/wiki/Euclidean_distance)

Original implementation under the repository
[Apache-2.0 license](../../../LICENSE).
