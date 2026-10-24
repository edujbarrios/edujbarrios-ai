# Sorted Postings List Union

## AI motivation

A Boolean **OR** query in an inverted index finds documents containing
at least one queried term. When both terms have ascending document-ID
postings lists, a two-pointer merge computes their union without
scanning the full corpus. This complements the repository's
`postings_list` and `postings_intersection` (AND) implementations
(Manning et al., 2008).

## Mathematical algorithm

Given **strictly increasing**, unique ID arrays `A` and `B`,
maintain pointers `i,j`. While both have entries, emit the smaller
value and advance its pointer. When values are equal, emit it **once**
and advance both. Finally append remaining entries from either list.

```text
A=[0,2,4]     B=[1,2,5]
union(A,B) = [0,1,2,4,5]
```

IDs remain zero-based and sorted. Duplicates **across lists** are
removed; duplicates **within a list** are invalid and rejected.

## Shared contract

- Both inputs are ascending arrays of **unique integer document IDs**
  in the inclusive range `[0, 2,147,483,647]`.
- Accept an empty left/right list, including both empty. The union of
  two empty lists is empty; an empty/nonempty pair returns the nonempty
  input after validation.
- Reject duplicates or out-of-order elements in either input, negative
  IDs, fractional IDs, NaN/infinity or IDs above 2,147,483,647.
- C/Rust take `uint32_t` inputs (rejecting out-of-range values), R
  accepts finite numeric vectors representing integer IDs, and Python
  accepts Python integer lists/tuples (rejecting bools).
- All outputs contain **zero-based ascending unique document IDs**.
  Python/R return sequences and raise errors; Rust returns a `Result`.
  C requires caller-owned result capacity `n_left+n_right` when
  nonempty, plus a nonnull `out_count` pointer. Empty input arrays may
  be NULL when their lengths are zero; output may be NULL when both
  lengths are zero. C rejects overflowing length additions, and leaves
  count/output unchanged if either input is invalid. No input/output
  buffer aliasing is supported.
- This primitive merges **document IDs only**; term frequencies and
  per-document posting payloads are outside the API.

## Complexity

**O(n+m)** time for lists of size `n` and `m`.
The output has up to `n+m` IDs; C uses O(1) additional space
beyond the caller-provided output. Python/Rust/R use O(n+m) memory
for the result.

## Implementations

| Language | Folder |
| --- | --- |
| C | [postings_union_c](postings_union_c/) |
| Python | [postings_union_python](postings_union_python/) |
| Rust | [postings_union_rust](postings_union_rust/) |
| R | [postings_union_r](postings_union_r/) |

Run the repository test suites with:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include overlaps, disjoint inputs, one/both empty lists,
boundary IDs and rejection of malformed lists even when the opposite
side is empty.

## Limitations

Two input lists only. Does not process raw text, tokenize queries,
build an index, merge frequencies, manage documents, compute TF-IDF/BM25,
support AND/NOT query syntax, or implement multiway query planning.
No CUDA version is justified without device testing and benchmarks.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
