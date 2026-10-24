# Two-Term Positional Phrase Search

## Why it belongs in AI

A conventional Boolean AND query finds documents containing both
terms, even when the terms occur far apart. **Positional inverted
indexes** store token offsets for each posting, allowing search
engines and retrieval systems to match exact phrases. This reference
implements a two-term ordered phrase query, complementing the
catalogue's postings list, union and intersection primitives
(Manning et al., 2008).

## Mathematical definition

Given sorted occurrence lists for a *first* and a *second* term,
represent each occurrence by a pair `(doc_id, position)`.
A document matches if there exists a token offset `p` with:

```text
(first_term,  doc_id, p)
(second_term, doc_id, p+1)
```

The order matters. The algorithm intersects documents using two
pointers, then tests positions within each shared document using
a linear merge. It returns matching **document IDs once each**,
sorted ascending.

Example:

```text
first  = [(1,0),(1,5),(2,1),(4,2)]
second = [(1,1),(1,8),(2,3),(4,3)]

matches = [1,4]
```

Document 2 contains both terms but does not have the adjacent
positions 1 and 2, so it does not match.

## Shared contract

- Each list contains **0..4096** `(doc_id,position)` occurrences.
  Both fields must be integer values in **[0,1,000,000,000]**.
- Occurrences in each input list must be **strictly increasing
  lexicographically by (doc_id,position)**, so the same pair
  cannot appear twice in one list. Multiple distinct positions
  within a document are allowed.
- Either list may be empty, producing an empty result. Input lists
  are validated even if the other list is empty.
- Return unique ascending **zero-based document IDs**, not positions,
  and do not mutate the postings. Same-document matches require the
  second position to be **exactly first+1**, not merely nearby.
- C accepts two parallel arrays of `uint32_t doc_ids[n]` and
  `uint32_t positions[n]` per list, plus caller-owned
  `uint32_t results[min(n_left,n_right)]` and a
  `size_t* result_count`. All nonempty arrays must be valid,
  output buffers cannot overlap inputs, and outputs stay unchanged
  on error. If a list is empty, NULL input pointers are accepted.
- Rust accepts sorted slices of `(u32,u32)` pairs, returning
  `Result<Vec<u32>,...>`. Python takes lists/tuples of integer
  pairs. R takes two-column integer/numeric matrices with
  columns `doc_id, position`; a 0-row matrix is valid.
- Python rejects booleans and fractional values. R rejects
  non-integer-valued numeric entries, NaN and negative values.

## Complexity

**O(n+m)** time for n first-term and m second-term occurrences
and **O(min(n,m))** worst-case output space; the reference performs
no expensive document-wide nested loops. Within a common document,
the two positional pointers advance monotonically.

## Implementations

| Language | Directory |
| --- | --- |
| C | [positional_phrase_search_c](positional_phrase_search_c/) |
| Python | [positional_phrase_search_python](positional_phrase_search_python/) |
| Rust | [positional_phrase_search_rust](positional_phrase_search_rust/) |
| R | [positional_phrase_search_r](positional_phrase_search_r/) |

Test from root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover matches spanning multiple documents, a shared document
without adjacency, multiple occurrences within one document,
empty inputs, unsorted and duplicate postings, invalid data and
reverse-order nonmatches.

## Limitations

Exactly **two query terms** separated by exactly one token position.
No tokenizer, stemming, offset normalization, phrase slop,
proximity scoring, skip pointers, compressed postings or
multi-term phrase optimization. Input postings must already be built
consistently by a retrieval index. No GPU optimization is claimed.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008).
*Introduction to information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
