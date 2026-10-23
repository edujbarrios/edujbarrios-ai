# Recall at K (R@K)

## Why it belongs in the AI hub

Retrieval-based AI and search engines require evaluation metrics that measure
what fraction of relevant documents have been recovered, not only the purity
of top results. Recall at K complements the existing Precision at K
algorithm, which does not require the total number of relevant documents.
The distinction is described by Manning et al. (2008).

## Definition

For a query with `total_relevant > 0` relevant documents in the assessed
corpus, and a ranked list of binary relevance labels:

```text
Recall@k = (sum(relevance[0:k])) / total_relevant
```

Example: ranking `[1, 0, 1, 0]`, `total_relevant=3`,
`k=3` yields `2/3`.

## Shared contract

- Input: a nonempty ranked sequence of labels **exactly 0 or 1**.
  Rank one is the first element.
- `k` must be an integer in `[1, n]`.
- `total_relevant` must be a **positive integer** representing all
  relevant documents for the query in the assessed corpus.
- The entire supplied ranking is validated, including positions **after k**.
  The number of relevant labels in the supplied ranking cannot exceed
  `total_relevant`.
- Output is a finite scalar in `[0,1]`. An empty top-k set is invalid;
  zero relevant documents among the first k returns 0.
- Queries with `total_relevant=0` return an error because recall would be
  undefined. A caller may choose a separate evaluation policy for such
  queries, but it is deliberately not silently inferred here.
- C accepts a caller-owned `uint8_t` array and an output pointer, and does
  not modify output on validation error. C cannot infer buffer sizes and
  requires at least `n` elements in the input. Python/R raise errors;
  Rust returns `Result<f64, ...>`.
- The ranking is assumed to contain **distinct retrieved documents**.
  This primitive does not inspect document IDs or resolve duplicate hits.

## Complexity

**O(n)** time because every label is validated; **O(1)** auxiliary space
in all four languages.

## Implementations

| Language | Folder |
| --- | --- |
| C | [recall_at_k_c/](recall_at_k_c/) |
| Python | [recall_at_k_python/](recall_at_k_python/) |
| Rust | [recall_at_k_rust/](recall_at_k_rust/) |
| R | [recall_at_k_r/](recall_at_k_r/) |

CUDA is not included: evaluating a single short ranking on GPU would be
misleading without batching and performance measurements.

## Run tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests check known results, missing relevant documents, zero top-k relevance,
invalid labels after k, incomplete denominator metadata, and invalid k.

## Limitations

This is binary-relevance recall over a single evaluated query. It does not
handle graded gain, query aggregation, incomplete relevance judgments,
document deduplication or confidence intervals. For meaningful benchmark
results, report how ground truth relevance was established.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
