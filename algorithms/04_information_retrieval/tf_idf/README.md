# TF-IDF: Single-Term Weight

## Why it belongs in this repository

Term frequency–inverse document frequency (TF-IDF) is a standard lexical
weighting building block in information retrieval, text features and
document/query vector representations. It increases the weight of a term
with its frequency in a document while reducing the importance of terms
common across the corpus (Manning et al., 2008).

This implementation returns the **weight of one term in one document**.
It is not a tokenizer, corpus indexer, vectorizer or ranking engine.

## Formula

Given a raw term frequency `tf`, corpus size `N`, and document frequency
`df`:

```text
idf(N, df) = ln(N / df)
tf_idf(tf, N, df) = tf * idf(N, df)
```

This explicitly uses **unsmoothed natural-log IDF** with raw count TF.
Other variants (log-scaled TF, smoothed IDF, length normalization) give
different results and should be separate implementations or configurable
extensions rather than silently changed defaults.

Examples:
- `tf=3, N=100, df=10` gives `3 * ln(10)`.
- `tf=0` gives zero.
- `df=N` gives zero inverse-document-frequency weight.

## Input and error contract

- `tf` is an integer `>=0`; `N` is an integer `>=1`; `df` is
  an integer with `1 <= df <= N`. `df=0` is rejected rather than
  implicitly assigning infinite IDF.
- All results must be finite and nonnegative. Invalid counts or
  unrepresentable products are errors.
- In C, counts are `size_t`, the function returns `bool` and the
  caller-owned output scalar is unchanged on failure.
- Python and R raise errors; Rust returns `Result<f64, ...>`.
- R supports exact nonnegative integer counts up to `2^53`, the maximum
  integer range safely represented in base R doubles.

## Complexity

**O(1)** time and **O(1)** additional memory per term. A real TF-IDF
vectorizer must collect corpus statistics and calculate weights across
many terms; that end-to-end cost is outside this algorithm's scope.

## Implementations

| Language | Folder |
| --- | --- |
| C | [tf_idf_c/](tf_idf_c/) |
| Python | [tf_idf_python/](tf_idf_python/) |
| Rust | [tf_idf_rust/](tf_idf_rust/) |
| R | [tf_idf_r/](tf_idf_r/) |

No CUDA implementation is claimed for this scalar term-weight primitive.

## Tests

Run `python3 scripts/run_tests.py --strict` from the repository root.
The suites cover common and rare terms, absent terms, full-corpus document
frequency, zero counts, invalid `df`, and extreme parameter behavior.

## Limitations

No tokenizer, smoothing, sublinear TF, document-length normalization,
stopword filtering, term-to-vector mapping, sparse storage or cosine scoring.
Weights depend on the corpus; using IDF computed from a different dataset
changes their meaning. This routine is not a substitute for a full retrieval
pipeline.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
