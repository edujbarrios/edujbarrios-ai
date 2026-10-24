# Hamming-Radius Signature Search

## AI motivation

A random-hyperplane locality-sensitive hash compresses a real vector
into a binary signature. With the repository's
`random_hyperplane_hash`, `lsh_bucket_index` and `hamming_top_k`
building blocks, another useful search operation is to retrieve **every
candidate within a chosen Hamming radius**, instead of asking for a
fixed number of neighbors. This illustrates multi-probe and candidate
expansion ideas for approximate-vector retrieval (Charikar, 2002).

This implementation performs **an exact scan over signatures**. It does
not generate signatures, create hash buckets, or guarantee nearest
neighbors under cosine similarity in the original vector space.

## Definition

For 30-bit nonnegative packed integers `q` and `s_i`:

```text
hamming(q,s_i) = popcount(q XOR s_i)
matches = {i : hamming(q,s_i) <= radius}
```

Return every matching **zero-based original index**, ordered first by
ascending distance, then by ascending index.

Example:

```text
query      = 0b0011
signatures = [0b0011, 0b0001, 0b0111, 0b1111, 0b0010]
distances  = [0, 1, 1, 2, 1]
radius = 1   -> indices [0, 1, 2, 4]
radius = 0   -> indices [0]
radius = 2   -> indices [0, 1, 2, 4, 3]
```

## Shared contract

- Query and each signature must be an integer in
  `[0, 2^30 - 1]`, matching the repository's 30-bit sign hash.
- Radius must be an integer in **[0, 30]**; zero is valid and selects
  exact signature matches.
- An **empty candidate collection is valid**, producing an empty result.
  For nonempty collections, duplicates are preserved with distinct
  indices. Validate all signatures, even those outside the radius.
- C accepts `uint32_t` signatures and radius, `size_t[n]` caller-owned
  indices (may be NULL if `n=0`), and required non-null `out_count`.
  On bad input all outputs stay unchanged; caller guarantees allocation
  and non-overlap. Rust returns `Result<Vec<usize>, ...>` with checked
  `u32` input and radius; Python and R raise errors.
- Python rejects booleans as integer inputs. R returns **zero-based**
  integer indices and rejects fractional/nonfinite/out-of-range inputs.
- A radius of 30 returns the complete collection sorted by distance.
  The bit order within the packed number is immaterial to popcount,
  but the same hash family must generate all signatures being compared.

## Complexity

A full scan of `n` signatures and a sort of the matching pairs
requires **O(n + M log M)** time and **O(M)** auxiliary memory, where
`M` is the number of results. In the worst case M=n, giving
O(n log n) time and O(n) memory. This does not provide sublinear
retrieval performance.

## Implementations

| Language | Directory |
| --- | --- |
| C | [hamming_radius_search_c](hamming_radius_search_c/) |
| Python | [hamming_radius_search_python](hamming_radius_search_python/) |
| Rust | [hamming_radius_search_rust](hamming_radius_search_rust/) |
| R | [hamming_radius_search_r](hamming_radius_search_r/) |

Run tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover radii 0, 1 and 30, duplicate signatures, empty candidate
collections, stable distance/index ordering, invalid range values and
validation of entries that are not returned.

## Limitations

The radius limits **Hamming distance**, not cosine or Euclidean
distance on original embeddings. Hash collisions and search recall
depend on how projections were selected. No hash-table acceleration,
multi-table LSH, recall benchmark, RAG pipeline or CUDA kernel is
included. Future approximate-search experiments should compare
candidates against the `exact_cosine_top_k` reference.

## References

Charikar, M. S. (2002). Similarity estimation techniques from rounding
algorithms. In *Proceedings of the Thirty-Fourth Annual ACM Symposium on
Theory of Computing* (pp. 380–388).
https://doi.org/10.1145/509907.509965
