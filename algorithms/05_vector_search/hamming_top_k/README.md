# Hamming-Distance Top-K Search

## AI motivation

Random-hyperplane locality-sensitive hashing (LSH) represents a vector as a
short binary signature. Candidate signatures can then be compared using
**Hamming distance**, the number of differing bits, before any optional
reranking using original embeddings. This algorithm complements the
repository's `random_hyperplane_hash` primitive and illustrates the
candidate-ranking step used alongside similarity hashing (Charikar, 2002).

**Scope:** Search exactly over the supplied signatures. This does not
generate hash signatures or implement hash tables, bucketing, approximate
nearest-neighbor guarantees, HNSW, or a vector database.

## Formula and ordering

For unsigned packed signatures `q` and `s_i`:

```text
distance(q, s_i) = popcount(q XOR s_i)
```

Return **zero-based indices** of the best `k` signatures, sorted by:
1. Ascending Hamming distance (smaller means more similar).
2. Ascending original index for equal distances, deterministically.

Example:
```text
query      = 0b0011
candidates = [0b0001, 0b0111, 0b1111, 0b0010, 0b0011]
distances  = [1, 1, 2, 1, 0]
top_4      = [4, 0, 1, 3]
```

## Contract

- A query and nonempty candidate array of **packed 30-bit nonnegative
  integers** in `[0, 2^30 - 1]`. This matches the repository's
  `random_hyperplane_hash` bit ordering and its maximum of 30 planes.
- `k` must be an integer in `[1, number_of_candidates]`.
- All candidate values are validated, including those that would not
  appear in the first `k` results.
- Duplicate candidate signatures are **kept** with distinct indices.
- C accepts `uint32_t` input and a caller-owned `size_t[k]` output.
  Buffers must be appropriately sized and non-overlapping; errors leave
  the output unchanged. Rust returns `Result<Vec<usize>, &str>`;
  Python and R raise errors. R returns zero-based integer indices, not
  its usual one-based indexing.
- Python rejects bools as integers; R rejects fractional, nonfinite and
  out-of-range numeric signatures/k values.
- Results are independent of platform endianness because bits are
  counted arithmetically, not by reinterpreting byte buffers.

## Complexity

Compute `popcount` for each signature and sort `n` (distance, index)
pairs: **O(n log n)** time, **O(n)** auxiliary memory. This is a
full-sort educational reference rather than heap-based `O(n log k)`
selection. No speedup is promised over exact vector search.

## Language implementations

| Language | Path |
| --- | --- |
| C | [hamming_top_k_c](hamming_top_k_c/) |
| Python | [hamming_top_k_python](hamming_top_k_python/) |
| Rust | [hamming_top_k_rust](hamming_top_k_rust/) |
| R | [hamming_top_k_r](hamming_top_k_r/) |

Run the complete CPU tests from the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover ties, identical signatures, duplicate entries, all 30 bits,
invalid signatures after `k`, empty input and invalid `k`.

## Limitations

Binary signatures are lossy; Hamming distance is **not** the original
cosine or Euclidean distance. Recall depends on how the hash family and
index are constructed. This function does not re-rank using original
vectors, perform multi-table probing, estimate collision rates or run on
a GPU. A CUDA version would require device testing and benchmarks.

## References

Charikar, M. S. (2002). Similarity estimation techniques from rounding
algorithms. In *Proceedings of the Thirty-Fourth Annual ACM Symposium on
Theory of Computing* (pp. 380–388).
https://doi.org/10.1145/509907.509965
