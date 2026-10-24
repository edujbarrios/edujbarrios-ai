# Exact Maximum Inner-Product Top-K Search (MIPS)

## AI motivation

**Maximum inner-product search (MIPS)** ranks embeddings according to
their dot product with a query vector. It is used in retrieval and
recommendation systems, including latent-factor recommendation where
vector **magnitude** can carry useful information. This differs from
cosine similarity, which removes magnitude by normalization, and
Euclidean distance, which orders vectors by a geometric metric.

This is an **exact brute-force search** suitable as a correctness
baseline for approximate vector indexing (Shrivastava & Li, 2014).

## Mathematics

Given a query `q∈R^d` and n candidate rows `x_i∈R^d`:

```text
score(i) = dot(q, x_i) = sum_{j=0}^{d-1} q[j] * x_i[j]
result = k indices with the highest scores
```

Sort by **descending dot product**, then **ascending original
zero-based index** when scores are exactly tied. Duplicate candidate
vectors are distinct rows, and the query can be all zero.

Example:

```text
query = [1,2]
rows  = [[1,0],[0,1],[1,2],[-1,0],[1,2]]
scores= [1,2,5,-1,5]
top_5 = [2,4,1,0,3]
```

No normalization is applied: a high-norm candidate may outrank a
closer directional neighbor.

## Shared contract

- Nonempty query with `d>=1`, nonempty rectangular `n×d`
  candidate matrix, and integer `1<=k<=n`.
- Every query coordinate and **every** candidate coordinate must be
  finite real. Validate all rows, even when k is small.
- Reject any nonfinite individual multiplication or accumulation.
  Finite inputs can overflow when multiplied or added; those cases
  are errors instead of silently ranking infinities.
- Return k unique original row **indices** in score order. Ties
  choose the lowest original index. The output contains **no**
  similarity scores and is not normalized.
- C and Rust accept flat **row-major** matrices; Python accepts
  nested numeric lists/tuples; R accepts a base numeric matrix.
  C uses caller-owned `size_t indices[k]`, nonoverlapping input
  and output arrays, and leaves the output unmodified on failure.
  Rust returns `Result<Vec<usize>,...>`, Python a list, R an
  integer vector of zero-based row indices.
- Python rejects bool coordinates or k. R requires an integer-valued
  finite k. All implementations accept negative vector coordinates,
  duplicate vectors and zero vectors.
- The dot product accumulates left to right through dimensions to
  expose one clear floating-point contract. Differences in floating
  arithmetic across platforms can affect extremely close rankings.

## Complexity

A full scan costs **O(n*d)**; sorting n scores costs
**O(n log n)** time. Uses O(n) additional storage beyond
the candidate matrix and query.

## Implementations

| Language | Directory |
| --- | --- |
| C | [maximum_inner_product_top_k_c](maximum_inner_product_top_k_c/) |
| Python | [maximum_inner_product_top_k_python](maximum_inner_product_top_k_python/) |
| Rust | [maximum_inner_product_top_k_rust](maximum_inner_product_top_k_rust/) |
| R | [maximum_inner_product_top_k_r](maximum_inner_product_top_k_r/) |

Tests from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover score ordering, ties, duplicate vectors, zero query,
invalid dimensions/cutoff, invalid candidates outside the top-k
set, product overflow and sum overflow.

## Limitations

No approximate nearest-neighbor structure, index construction,
quantization, batch queries, SIMD benchmark or GPU implementation.
Unlike cosine search, magnitude affects scores; input preprocessing
should reflect the chosen retrieval task. This implementation is an
exact baseline, not a low-latency production vector index.

## References

Shrivastava, A., & Li, P. (2014). Asymmetric LSH (ALSH) for sublinear
time maximum inner product search (MIPS). In *Advances in Neural
Information Processing Systems* (Vol. 27).
https://proceedings.neurips.cc/paper/2014/hash/d072677d210ac4c03ba046120f0802ec-Abstract.html
