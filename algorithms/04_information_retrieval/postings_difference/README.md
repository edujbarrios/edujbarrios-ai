# Sorted Postings List Difference (AND NOT)

## Why it belongs in the AI hub

An inverted index supports Boolean document retrieval by combining
sorted postings lists. After **intersection** (AND) and **union** (OR),
a useful complement is **set difference** (AND NOT): find documents
containing a required term while excluding those matching another.
Merging sorted postings lists avoids scanning every document
(Manning et al., 2008).

## Definition

For strictly increasing, unique document-ID lists `included` and
`excluded`, return their **asymmetric difference**:

```text
difference(included, excluded) = included \ excluded

included = [0,2,4,7]
excluded = [1,2,5,7]
result   = [0,4]
```

Advance two indices through both sorted lists. Emit an included value
when it is smaller than the current excluded value; skip both on a
match; otherwise advance the excluded index. Emit remaining included
IDs after the excluded list ends.

## Shared contract

- Inputs contain **strictly increasing, unique** integer document IDs
  in `[0,2^31-1]`. No sorting/deduplication is performed silently.
- Either or both lists may be empty, producing the original included
  list or an empty list as appropriate.
- Reject duplicates, descending input, negative, fractional, nonfinite
  and out-of-range document IDs. Even if included is empty, excluded
  must still be validated.
- Output is a sorted unique list of **zero-based** document IDs that
  appear in the included list and do not appear in excluded.
- C accepts `uint32_t` arrays and caller-owned output capacity
  **at least n_included**; when n_included is zero output may be NULL.
  The `out_count` pointer is required. Invalid input leaves count
  and output unchanged. No input/output aliasing is supported.
  Rust returns `Result<Vec<u32>,...>` using unsigned input IDs.
  Python returns a list and R an integer vector.
- R/Python reject malformed dynamically typed inputs. Python rejects
  booleans as IDs; R accepts finite integral numeric vectors within
  the common ID range.
- Difference is **not commutative**. It operates on document IDs only,
  not posting frequencies, offsets or Boolean query syntax.

## Complexity

**O(n+m)** time for included and excluded lists of lengths `n`, `m`.
The output requires at most O(n) elements. C uses O(1) auxiliary memory
beyond caller-owned output; Rust/Python/R use O(n) output storage.

## Implementations

| Language | Folder |
| --- | --- |
| C | [postings_difference_c](postings_difference_c/) |
| Python | [postings_difference_python](postings_difference_python/) |
| Rust | [postings_difference_rust](postings_difference_rust/) |
| R | [postings_difference_r](postings_difference_r/) |

Run tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover overlapping/disjoint lists, same lists, both/one empty,
maximum supported document IDs, malformed lists on either side, and
preservation of C output on validation errors.

## Limitations

Only two sorted posting ID lists; no raw-text tokenization, term-
frequency merging, document-level negation relative to an entire
collection, multiway NOT planning, skip pointers or query parser.
A standalone CUDA implementation would require its own device tests
and performance assessment.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
