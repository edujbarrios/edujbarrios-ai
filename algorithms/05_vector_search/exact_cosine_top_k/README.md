# Exact Cosine Top-K Vector Search

## Why it belongs in the AI hub

Cosine similarity compares **vector directions** rather than magnitudes.
It is used in information retrieval and embedding similarity search
(Manning et al., 2008). This algorithm ranks a whole collection against
one query without approximation. It can serve as a **ground-truth
baseline** for later experiments with LSH, quantization and approximate
nearest-neighbor indexes.

Unlike the repository's single-pair `cosine_similarity` primitive,
this algorithm returns the indices of the **k highest-scoring vectors**
with a defined tie-breaking rule.

## Mathematical definition

For query `q` and candidate vector `x_i`, each in `R^d`:

```text
cosine(q, x_i) = sum_j (q_j / ||q||) * (x_ij / ||x_i||)
||v|| = sqrt(sum_j v_j^2)
```

We compute norms using an iterative hypotenuse recurrence, avoiding
avoidable intermediate squares such as `(1e200)^2`. Dividing each
component by its own norm before multiplying also avoids forming a
possibly overflowing product `||q|| * ||x_i||`.

Return **zero-based indices** ordered by:
1. **Descending** cosine score.
2. **Ascending** original candidate index on exactly equal scores.

Example:

```text
query      = [1,0]
candidates = [[1,0], [0,1], [-1,0], [1,1], [1,0]]
cosines    = [1, 0, -1, 0.7071..., 1]
top_4      = [0, 4, 3, 1]
```

## Shared contract

- One nonempty, nonzero, finite real query vector of dimension `d>=1`.
- A nonempty **n × d** dense matrix of nonzero finite candidate vectors.
- An integer `k` satisfying `1 <= k <= n`.
- Every candidate must be valid, including those not in the returned Top-K.
  Reject zero-norm vectors, NaN/infinity, wrong dimensions and numerical
  overflow in norm or score accumulation.
- Rounding can move a computed cosine infinitesimally outside [-1,1];
  scores are clamped to this interval before comparison. Signed zero
  cosine values compare as ties.
- C takes row-major flat candidate storage, with caller-provided index
  output of capacity `k`; buffers must not overlap. On any failure C
  leaves the output unchanged. Rust also takes row-major flat vectors
  and returns `Result<Vec<usize>, ...>`. Python accepts nested row
  vectors, and R accepts a base matrix; both return zero-based integer
  indices and raise errors for malformed data.
- The code performs a **full scan and full sort**. It makes no indexing
  or approximate-search performance claim.

## Complexity

For `n` candidate vectors each of dimension `d`:
**O(n*d + n log n)** time and **O(n)** auxiliary memory for the score-index
pairs. Python additionally converts/copies its matrix (O(n*d) memory).
Output size is O(k). Results have no trained state.

## Language implementations

| Language | Directory |
| --- | --- |
| C | [exact_cosine_top_k_c](exact_cosine_top_k_c/) |
| Python | [exact_cosine_top_k_python](exact_cosine_top_k_python/) |
| Rust | [exact_cosine_top_k_rust](exact_cosine_top_k_rust/) |
| R | [exact_cosine_top_k_r](exact_cosine_top_k_r/) |

Run the repository's strict CPU tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover unit vectors, exact ties, identical entries, negative
similarities, dimension mismatches, zero vectors, nonfinite inputs and
overflowing vector norms.

## Limitations

This is an **exact brute-force reference**, not an approximate index.
Its scan/sort cost may be high for large corpora. No batch queries,
streaming/online index updates, graph navigation, IVF, product
quantization, CUDA kernels or cross-encoder reranking. Finite vector
components can still produce a nonrepresentable Euclidean norm, which
is explicitly rejected. No claim that cosine retrieval measures semantic
relevance without a suitable embedding model.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
