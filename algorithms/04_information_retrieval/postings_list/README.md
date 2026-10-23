# Single-Term Inverted Postings List

## Why it belongs in the AI hub

An **inverted index** maps each term to documents containing it. Each
term's postings list is a core component of lexical retrieval, BM25
scoring, and sparse retrieval for RAG. This implementation builds the
postings for **one queried token ID** from a collection of documents,
without relying on a search framework (Manning et al., 2008).

It is intentionally a small building block: an index for **all** tokens
would maintain many such postings lists and an efficient vocabulary map.

## What it does

For a corpus of ordered documents and a target integer token ID, return
one posting per document containing the term:

```text
documents = [[1,2,1], [], [2,1], [3]]
target = 1
postings = [(document_id=0, frequency=2), (document_id=2, frequency=1)]
```

Document IDs are **zero-based**, in ascending order. Duplicate token
occurrences within a document **increase the frequency** rather than
creating duplicate postings.

## Shared contract

- Token IDs are signed **32-bit integers** in the inclusive range
  `[-2,147,483,648, 2,147,483,647]`; negative IDs are allowed.
- Accept an empty document collection, empty documents, and a term absent
  from the corpus; each produces an empty postings list.
- Outputs are ordered `(document_id, frequency)` pairs, where every
  frequency is a positive integer. No full vocabulary or index is built.
- Python takes a list/tuple of token-ID lists/tuples and raises
  `ValueError` for noninteger, bool, out-of-range or malformed input.
- Rust takes `&[Vec<i32>]` and returns `Vec<(usize, usize)>`.
- R takes a list of numeric vectors and returns a list containing
  `doc_ids` and `frequencies`, both integer vectors. Each token is
  validated for finite, integral and signed-32-bit range. Invalid input
  produces an error.
- C takes flattened row-major (document-major) token IDs, an `offsets`
  array of **n_documents+1** monotonically increasing positions, and a
  target `int32_t`. `offsets[0]=0` and
  `offsets[n_documents]=n_tokens`. The output arrays must each contain
  at least `n_documents` positions (or may be NULL when `n_documents=0`).
  C writes `out_count` and posting arrays **only after validation**.
  The caller is responsible for all buffer capacities and non-aliasing.
- Empty C corpus: n_documents=0, n_tokens=0, offsets[0]=0; token pointer
  and posting output arrays can be NULL, but `out_count` cannot be NULL.
- No strings are tokenized; the caller is responsible for assigning the
  same token ID to equivalent terms across documents.

## Complexity

Let `T` be the total token count, `D` the number of documents, and
`M` the number of matching documents. Full validation and scan require
**O(T + D)** time, producing **O(M)** output storage. The C implementation
uses O(1) additional working memory excluding caller-provided output.

## Implementations

| Language | Folder |
| --- | --- |
| C | [postings_list_c](postings_list_c/) |
| Python | [postings_list_python](postings_list_python/) |
| Rust | [postings_list_rust](postings_list_rust/) |
| R | [postings_list_r](postings_list_r/) |

## Examples and tests

```text
postings([[1,2,1],[],[2,1],[3]], target=1) -> [(0,2),(2,1)]
postings([[1,2,1]], target=4)              -> []
postings([], target=-4)                    -> []
```

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from the repository root.
Tests cover document order, repeated tokens, absent terms, empty corpora,
negative IDs, invalid token values and invalid C offsets.

## Limitations

A single-term scan is not a production index and does **not** provide
fast repeated query lookup. No persistent storage, full inverted index,
tokenization, skip pointers, compression, intersections, dynamic
document updates or BM25 scoring are implemented here. These are good
subjects for future focused PRs.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008).
*Introduction to information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
