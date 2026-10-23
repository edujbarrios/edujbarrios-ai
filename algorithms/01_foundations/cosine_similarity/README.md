# Cosine Similarity

**Stage:** 01 — Foundations · **Languages:** C, Python, Rust, R.

## Why it belongs in this repository

Cosine similarity compares the direction of two vectors rather than their
raw magnitude. In information retrieval, it is used to compare document and
query representations; it is also common in embedding search (Manning et al.,
2008). It builds on dot products and normalization.

## What it does

For nonzero vectors `a` and `b` of the same dimension:

```text
cosine(a, b) = dot(a, b) / (||a||₂ × ||b||₂)
```

For example `[1, 0]` and `[0, 1]` are orthogonal (similarity 0);
`[1, 2]` and `[2, 4]` share direction (similarity 1).

## Shared contract

- Input: two equal-length sequences of finite real numbers.
- Output: one finite scalar in **[-1, 1]** (small rounding excursions are clamped).
- Empty vectors or either all-zero vector: **error**, because direction is undefined.
- Mismatched dimensions, NaN, infinity and non-finite calculations: **error**.
- C returns a `bool`, preserving `*out` on failure. Since C pointers do not
  encode lengths, callers must supply two valid buffers with at least `n`
  doubles. Python and R raise errors; Rust returns `Result<f64, ...>`.
- To avoid overflow in norms or dot products, first scale each vector by its
  maximum absolute element. The scaled products and squared components are
  accumulated and divided by the scaled norms. Very small components can
  underflow to zero; implementations are not mathematically exact.
- Numerical comparisons should use a tolerance, not exact equality.

## Complexity

Two scans per vector: **O(n)** time. C, Python, and Rust use **O(1)**
auxiliary memory. R's vectorized arithmetic allocates **O(n)** temporary
arrays. No AI or numerical framework dependency is required.

## Implementations

| Language | Location | Testing |
| --- | --- | --- |
| C | [cosine_similarity_c](cosine_similarity_c/) | C11 + libm |
| Python | [cosine_similarity_python](cosine_similarity_python/) | unittest |
| Rust | [cosine_similarity_rust](cosine_similarity_rust/) | rustc --test |
| R | [cosine_similarity_r](cosine_similarity_r/) | Rscript |

CUDA is **not implemented** in this PR; a GPU implementation needs meaningful
batching, benchmarks and hardware verification rather than merely duplicating
a one-vector CPU routine.

## Examples and tests

```text
cosine([1, 2], [2, 4])  → 1
cosine([1, 0], [0, 1])  → 0
cosine([1, 0], [-1, 0]) → -1
```

Run all declared CPU tests from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

No batching, sparse vectors, SIMD, weights, approximate neighbors or
production-optimised accumulation. Input vectors must have nonzero norm.
The method measures angular alignment, not semantic truth or calibrated
similarity; a high score alone does not establish evidence relevance.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/

Strang, G. (2016). *Introduction to linear algebra* (5th ed.).
Wellesley-Cambridge Press.
