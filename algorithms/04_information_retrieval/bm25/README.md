# BM25: Single-Term Scoring Contribution

## Why this algorithm is in the AI hub

BM25 is a foundational lexical-retrieval scoring method for search systems
and retrieval-augmented generation (RAG). It weighs a query term using
corpus rarity, its frequency in a document, and document-length normalization.
The probabilistic relevance formulation is discussed by Robertson and
Zaragoza (2009).

**Scope:** This implementation computes the contribution of **one distinct
query term to one document**. A full search system must tokenize its corpus,
build an inverted index, maintain global document-frequency statistics,
and sum term contributions for a multi-term query. This is not a complete
retrieval index or a ranking pipeline.

## Formula

For one term:

```text
idf = ln(1 + (N - df + 0.5) / (df + 0.5))
normalizer = k1 * (1 - b + b * document_length / avg_document_length)
score = idf * tf * (k1 + 1) / (tf + normalizer)
```

Inputs: `N` is the number of documents, `df` the term's document
frequency, `tf` its count in this document, `document_length` the
document token count, and `avg_document_length` the positive corpus
average. The constants satisfy `k1 > 0` and `0 <= b <= 1`.
The IDF uses the nonnegative `log(1+...) ` variant.

## Contract and validation

- Integer counts: `N >= 1`, `0 <= df <= N`, `0 <= tf <= document_length`.
  All counts must be nonnegative integers. A document length of zero is
  permitted only with `tf = 0`.
- `avg_document_length > 0`, `k1 > 0`, and `b` in `[0,1]`, all finite.
- If `tf = 0`, the score is exactly 0, after validating the inputs.
- Reject non-finite intermediate arithmetic and invalid statistics.
  A score is guaranteed nonnegative when the inputs are valid.
- C returns `bool` and only sets the output scalar on success;
  Python and R raise errors; Rust uses `Result<f64, ...>`.
- R represents counts as doubles: exact integer counts beyond `2^53`
  are not supported in the R reference implementation.
- The function does not infer corpus statistics or tokenize query strings.

## Complexity

**O(1)** time and **O(1)** space per term/document contribution. Computing
a document's complete query score requires iterating through query terms.

## Implementations

| Language | Directory |
| --- | --- |
| C | [bm25_c/](bm25_c/) |
| Python | [bm25_python/](bm25_python/) |
| Rust | [bm25_rust/](bm25_rust/) |
| R | [bm25_r/](bm25_r/) |

No CUDA implementation is claimed. Building a GPU retrieval system requires
a different end-to-end design and hardware benchmarks.

## Example and tests

For `N=100`, `df=10`, `tf=3`, `document_length=100`,
`avg_document_length=100`, `k1=1.2`, `b=0.75`,
the score is approximately `3.557` (computed from the formula).

Run `python3 scripts/run_tests.py --strict` from the repository root.
Tests exercise absent terms, length normalization, document frequency,
invalid parameters, invalid counts and finite results.

## Limitations

No inverted index, stemming, tokenization, multi-term aggregation, stopword
policy, field weighting (BM25F), or automatic corpus statistics. The formula
and parameters here must match those used by any downstream retrieval system.

## References

Robertson, S., & Zaragoza, H. (2009). The probabilistic relevance framework:
BM25 and beyond. *Foundations and Trends in Information Retrieval, 3*(4),
333–389. https://doi.org/10.1561/1500000019
