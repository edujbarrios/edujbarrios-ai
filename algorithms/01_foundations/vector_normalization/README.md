# Vector Normalization (L2)

**Stage:** 01 — Foundations · **Languages:** C, Python, Rust, R.

## Why it belongs in this repository

Many AI methods compare vector directions rather than raw magnitudes.
L2 normalization creates a **unit vector** that can be used in cosine-based
embedding search and similarity computations. It is a foundational vector
operation studied in linear algebra (Strang, 2016).

## What it does

For a nonzero vector `x`:

```text
||x||₂ = sqrt(sum(x[i]²))
normalize(x)[i] = x[i] / ||x||₂
```

Example: `[3, 4]` becomes `[0.6, 0.8]`, with norm 1.

## Shared contract

- Input: one **nonempty**, finite real vector with at least one nonzero element.
- Output: a new vector with the same dimensionality and (approximately) unit
  Euclidean norm. Signs are preserved.
- Reject zero vectors, empty vectors, NaN or infinity.
- Scaling by the maximum absolute input value before accumulating squared
  magnitudes avoids overflow from a naive `x[i] * x[i]` computation.
- Very small normalized elements may underflow to zero in IEEE-754; this is
  documented behavior, not a guarantee of exact mathematical normalization.
- Python/R raise an error; Rust returns `Result<Vec<f64>, ...>`. C takes
  a caller-supplied output buffer and returns `bool`, with no writes on
  failure. C callers are responsible for valid input/output buffer lengths
  of at least `n`; an identical input/output buffer is allowed.

## Complexity

All implementations take **O(n)** time. The C implementation uses O(1)
working memory beyond its caller-provided output; Python and Rust allocate
an O(n) output; R uses O(n) vectorized temporaries and output.

## Implementations

| Language | Directory | Required tools |
| --- | --- | --- |
| C | [vector_normalization_c](vector_normalization_c/) | C11, libm |
| Python | [vector_normalization_python](vector_normalization_python/) | Python 3.10+ |
| Rust | [vector_normalization_rust](vector_normalization_rust/) | Rust 2021 |
| R | [vector_normalization_r](vector_normalization_r/) | Base R |

CUDA is not included here. A separate CUDA PR would need batch-oriented tests
and benchmarks to demonstrate meaningful acceleration.

## Examples and tests

```text
normalize([3, 4])     -> [0.6, 0.8]
normalize([-3, 4])    -> [-0.6, 0.8]
normalize([0, 0])     -> error (undefined direction)
normalize([1e308])   -> [1] (no intermediate overflow)
```

Run the repository's CPU checks:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

This is a dense, CPU reference implementation, not an optimized BLAS or
embedding-serving component. It does not handle sparse arrays, broadcasting,
batches, missing values, complex numbers or zero-vector fallback policies.

## References

Strang, G. (2016). *Introduction to linear algebra* (5th ed.).
Wellesley-Cambridge Press.
