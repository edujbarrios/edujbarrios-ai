# LSH Bucket Index (Exact Signature Matches)

## AI motivation

**Locality-sensitive hashing (LSH)** hashes vectors so that similar inputs
are likely to collide into the same signature bucket. A bucket index can
retrieve candidate vectors without scanning the entire corpus. It is a
common building block for approximate similarity retrieval; the collision
probabilities depend on the hash family and its parameters (Charikar, 2002).

This algorithm builds a **reusable, static, exact-bucket index** over
**existing packed 30-bit signatures** produced by the repository's
`random_hyperplane_hash` algorithm. The query looks up one signature
bucket, returning original candidate indices for downstream reranking.
Unlike the `hamming_top_k` primitive, it does **not** search neighboring
signatures or return k closest signatures.

## Index representation and query

Given corpus signatures `s[0..n-1]`, build a mapping:

```text
bucket[signature] = [original index of every exact signature occurrence]
```

For efficient queries, C and R sort (signature, original-index) pairs,
then use binary search for the first matching signature. Rust uses a
`BTreeMap`; Python uses a dictionary; all index structures are reusable.

Example:

```text
signatures = [3, 7, 3, 0, 7]
lookup(3) -> [0, 2]
lookup(7) -> [1, 4]
lookup(4) -> []
```

Original insertion order is preserved **within each bucket**, including
repeated signatures. A lookup does not mutate the index.

## Shared contract

- Nonempty corpus of integers in `[0, 2^30-1]` (30 bits), consistent
  with `random_hyperplane_hash`. Repeated signatures are valid.
- A query signature must also be within this range. Absent buckets produce
  an **empty result**, not an error.
- Results are **zero-based original positions** in ascending order.
- Build validates every signature; invalid input is rejected rather than
  silently masking high bits.
- Python raises `ValueError` and exposes
  `build_lsh_bucket_index(signatures)` returning an index object with a
  `.query(signature)` method. Rust exposes `LshBucketIndex::build`
  and `.query`, each returning `Result`.
- C exposes `lsh_bucket_index_build`, `lsh_bucket_index_query`, and
  `lsh_bucket_index_free`. Build yields an owning opaque heap index;
  free it after use. Caller supplies `output[n]` and `out_count`;
  query leaves them unchanged on error. Build leaves its pointer output
  unchanged on failure. Caller buffers must have correct sizes and
  must not overlap index storage.
- R returns a list containing sorted signatures and source indices;
  `lsh_bucket_index_query(index, query)` returns integer indices.
  Inputs in R must be finite, nonnegative integral numbers.
- This index is static: it does not support deletion, insertion, hashing
  original float vectors, per-query random hyperplanes, or persistence.
  Every query must use the **same ordered hyperplane family** as corpus
  signatures; mismatched hash families give meaningless buckets.

## Complexity

C/R: sort `n` signature-index pairs in **O(n log n)** build time;
exact lookup via lower-bound binary search plus output of `b` matches
takes **O(log n + b)** time, using O(n) index storage.

Python: expected O(n) build and **O(1 + b)** query with dictionary
hashing. Rust: O(n log u) build, O(log u + b) query where `u` is the
number of distinct buckets. Both use O(n) index storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [lsh_bucket_index_c](lsh_bucket_index_c/) |
| Python | [lsh_bucket_index_python](lsh_bucket_index_python/) |
| Rust | [lsh_bucket_index_rust](lsh_bucket_index_rust/) |
| R | [lsh_bucket_index_r](lsh_bucket_index_r/) |

Run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover duplicate signatures, ordered indices, missing buckets,
repeated queries, 30-bit range limits, invalid data, and C output
preservation on error.

## Limitations

Only **exact bucket** lookup—no multiprobe hashing, Hamming-distance
candidate expansion, multi-table fusion, cosine reranking, approximate
recall evaluation, online updates, persistent storage or GPU kernels.
LSH can miss similar vectors in different buckets. Validate search
quality against the repository's `exact_cosine_top_k` baseline.

## References

Charikar, M. S. (2002). Similarity estimation techniques from rounding
algorithms. In *Proceedings of the Thirty-Fourth Annual ACM Symposium on
Theory of Computing* (pp. 380–388).
https://doi.org/10.1145/509907.509965
