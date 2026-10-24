# Average Precision at K (AP@K)

## Why it belongs in AI

Average precision rewards retrieval systems that rank relevant documents
early. Unlike Precision@K, which only counts how many of the top K items
are relevant, **Average Precision at K** accounts for the position
of each relevant result. It complements this repository's Precision@K,
Recall@K, Mean Reciprocal Rank and NDCG evaluation primitives.
Average precision is a standard information-retrieval evaluation measure
(Manning et al., 2008).

## Mathematics and normalization

For a ranked list of `n` binary relevance indicators
`rel[i] ∈ {0,1}`, cutoff `1 <= k <= n`, and the **total number
of relevant items in the entire evaluation corpus** `R`:

```text
precision_at(i) = sum_{j=1..i}(rel[j]) / i

AP@K = sum_{i=1..k} (precision_at(i) * rel[i]) / min(R, k)
```

If `R=0`, define AP@K to be **0**. Require `R` to be at least
the number of relevant flags in the supplied ranked list (which can
be shorter than the corpus). This guards inconsistent metadata.

This particular `min(R,k)` normalization makes AP@K=1 achievable when
the top K are all relevant even if more relevant items exist outside
the cutoff. Other literature uses `R` as denominator even when
K is smaller; these definitions are **not interchangeable**.
Always compare values computed with the same normalization policy.

Example:

```text
relevance = [1,0,1,0,1]
R = 3

AP@2 = (1) / min(3,2)                  = 0.5
AP@4 = (1 + 2/3) / min(3,4)            = 5/9
AP@5 = (1 + 2/3 + 3/5) / min(3,5)     = 34/45
```

## Shared contract

- Inputs: a **nonempty**, ordered list of binary integer relevance
  values 0/1; integer cutoff `k` in `[1,n]`; integer
  `total_relevant R >= 0`.
- Require `R >= sum(rel)` **over the entire supplied list**, not
  just the first K. This catches invalid corpus relevance counts even
  when bad metadata is outside the evaluated cutoff.
- Return a finite double scalar in [0,1]; return 0 if `R=0`.
  An otherwise valid all-irrelevant list can have R=0 or a positive
  R (relevant results exist outside this ranking).
- No sorting or deduplication: the input is already in **retrieval rank
  order** (first element is rank 1). The model does not know doc IDs,
  so callers must ensure duplicate documents have been handled.
- C/Rust accept `uint8_t`/`u8` relevance flags; C writes a
  caller-owned output scalar **only on success**. Rust returns
  `Result<f64,...>`. Python rejects booleans and noninteger relevance
  flags; R accepts exact numeric 0/1 relevance values and integral
  cutoff/count scalars.
- R limits `R` to [0,2^31−1] for compatibility with its integer
  count representation; all implementations behave identically
  over this shared range.
- Arithmetic accumulates a double-valued precision sum and does
  not interpret raw relevance values as scores or probabilities.

## Complexity

**O(n)** time to validate the whole ranking plus O(k) time to compute
the precision-weighted relevance sum. O(1) auxiliary memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [average_precision_at_k_c](average_precision_at_k_c/) |
| Python | [average_precision_at_k_python](average_precision_at_k_python/) |
| Rust | [average_precision_at_k_rust](average_precision_at_k_rust/) |
| R | [average_precision_at_k_r](average_precision_at_k_r/) |

## Tests

Run from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the example at multiple cutoffs, perfect ranking,
no relevant documents, relevant docs outside K, inconsistent corpus
relevance counts, invalid binary flags and cutoff bounds.

## Limitations

Binary relevance only, no graded judgments, discounts, query aggregation
(mean AP over multiple queries), interpolated precision–recall curves,
document deduplication or confidence intervals. A corpus-wide R value
must be supplied by the evaluator; it is **not inferred** from the
truncated ranked list. No CUDA benchmark or GPU support is claimed.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
