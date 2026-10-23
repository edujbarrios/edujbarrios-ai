# Jaccard Similarity

## Why it belongs in the AI hub

Jaccard similarity measures overlap between sets. In information retrieval,
it can compare token or shingle sets and is a useful conceptual foundation
for near-duplicate detection and MinHash (Manning et al., 2008).
This example works on **integer token IDs** rather than natural-language
strings to keep semantics identical across C, Python, Rust and R.

## Mathematical definition

For sets `A` and `B`:

```text
J(A, B) = |A ∩ B| / |A ∪ B|
```

The value lies in `[0,1]`, with 1 meaning identical sets. By documented
convention, `J(empty, empty) = 1`; an empty and a nonempty set return 0.

The input may contain duplicate token IDs; duplicates are discarded before
counting. For `A=[1,1,2,3]` and `B=[2,3,4]`, similarity is `2/4=0.5`.

## Contract

- Input: two sequences of signed **32-bit integer IDs**, in any order,
  with duplicates permitted. Valid ID range is
  `[-2,147,483,648, 2,147,483,647]`.
- Return a finite real scalar in `[0,1]`. Both inputs can be empty.
- Python rejects bools/non-integers/out-of-range values; R accepts numeric
  vectors with finite whole-number IDs in range. C uses `int32_t` arrays;
  Rust uses `i32` slices. C/Rust typed inputs cannot contain invalid
  numeric types or exceed the ID range.
- C returns `bool` and only writes the output after computation succeeds.
  Raw input pointers must reference at least their declared lengths; null
  input is permitted only when its length is zero.
- **No hashing or string tokenization is performed.** External systems must
  assign integer IDs consistently across the compared sets.

## Complexity

Sort both arrays and merge unique sorted IDs: **O(n log n + m log m)** time,
**O(n + m)** auxiliary memory. This is a readable exact implementation,
not a sketching or approximate-similarity algorithm.

## Implementations

| Language | Folder |
| --- | --- |
| C | [jaccard_similarity_c](jaccard_similarity_c/) |
| Python | [jaccard_similarity_python](jaccard_similarity_python/) |
| Rust | [jaccard_similarity_rust](jaccard_similarity_rust/) |
| R | [jaccard_similarity_r](jaccard_similarity_r/) |

## Examples and tests

```text
jaccard([1,1,2,3], [2,3,4]) -> 0.5
jaccard([], [])             -> 1
jaccard([], [3])            -> 0
jaccard([-5, 3], [3, -5])   -> 1
```

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from the repository root.

## Limitations

This implementation does not compute text tokens, MinHash signatures,
weighted Jaccard similarity, multisets or approximate matches. The chosen
empty-set convention must be kept consistent in downstream evaluation.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008).
*Introduction to information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
