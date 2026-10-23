# Matrix Multiplication

**Stage:** Foundations · **Languages:** C, Python, Rust, R.

## Why it belongs here

Dense matrix multiplication underpins neural-network layers, linear
transformations, attention projections, and numerical machine learning.
It composes scalar multiplication and addition into a shape-aware operation
(Strang, 2016). This first reference implementation favors transparency over
GPU or SIMD optimization.

## Mathematical definition

For `A` with shape `m × k` and `B` with shape `k × n`:

```text
C[i, j] = sum(A[i, p] * B[p, j], p = 0 ... k-1)
C has shape m × n
```

Example: `[[1,2,3],[4,5,6]] @ [[7,8],[9,10],[11,12]]`
equals `[[58,64],[139,154]]`.

## Contract

- Inputs are **nonempty, rectangular** matrices with strictly positive
  dimensions, compatible inner dimensions, and finite real elements.
- Output is a newly allocated finite matrix of shape `m × n`.
- Invalid shapes, booleans-as-numbers, NaN, infinity, and any overflowing
  product or running sum are errors. No partial outputs on failure.
- Python accepts nested sequences and raises `ValueError`; R accepts two
  base-R matrix objects and raises an error; Rust returns
  `Result<Vec<f64>, &'static str>`. C returns `bool` and writes to a
  caller-provided row-major buffer only on success. C buffer sizes and
  non-overlap are caller obligations.
- C/Rust use flat **row-major** matrices. Python uses nested rows.
  R follows standard `matrix[i, j]` indexing (column-major internally).
- Rounding differs across languages; tests check simple exact examples and
  compare floating-point outputs with tolerances for non-integral cases.

## Complexity

The straightforward triple loop requires **O(mkn)** operations and **O(mn)**
output space. C uses **O(mn)** additional scratch space to protect the
destination from partial writes; Python, Rust and R allocate their output
of shape `m × n`.

## Implementations

| Language | Directory |
| --- | --- |
| C | [matrix_multiplication_c](matrix_multiplication_c/) |
| Python | [matrix_multiplication_python](matrix_multiplication_python/) |
| Rust | [matrix_multiplication_rust](matrix_multiplication_rust/) |
| R | [matrix_multiplication_r](matrix_multiplication_r/) |

CUDA is deliberately not claimed until a tested kernel and GPU benchmark are
available in a separate pull request.

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Covered: compatible rectangular matrices, 1×1 matrices, negative numbers,
mismatched shapes, empty matrices, non-finite input and overflow.

## Limitations

No BLAS dependency, GPU kernel, blocking, broadcasting, batched matrices,
mixed precision, autograd, sparse matrices or speed guarantees. This code
is a **reference**, not a substitute for a tuned GEMM implementation.

## References

Strang, G. (2016). *Introduction to linear algebra* (5th ed.).
Wellesley-Cambridge Press.
