# Mean Reciprocal Rank (MRR)

## Why it belongs in the AI hub

Retrieval-augmented generation, question answering, recommendation and
search applications often care about **how early the first relevant result
appears**. MRR measures the average reciprocal of that position across a
set of evaluated queries. It complements Precision at K and Recall at K.
Reciprocal rank is discussed in information-retrieval evaluation, including
Manning et al. (2008).

## Formula

For `q > 0` queries, let `rank_i` be the one-based position of the first
relevant result for query `i`. Use `rank_i = 0` when no relevant result
appears in the evaluated retrieval list:

```text
RR(i) = 0                 if rank_i == 0
        1 / rank_i        otherwise

MRR = sum(RR(i)) / q
```

Example: `[1, 2, 0, 4]` gives
`(1 + 0.5 + 0 + 0.25) / 4 = 0.4375`.

## Contract

- Input is a **nonempty** sequence of nonnegative integer ranks.
- Positive ranks are **one-based**; 0 denotes no relevant hit. A ranking
  position cannot be negative or fractional. Boolean values are not ranks.
- Output is a finite scalar in `[0, 1]`; all-zero ranks give `0`.
- Python and R reject invalid ranks and raise errors. Rust accepts `usize`
  ranks and returns `Result<f64, ...>`; C accepts a `size_t` array and
  returns `bool`, writing the output only after complete validation.
  Callers must provide a valid C array containing at least `n` ranks.
- C/Rust cannot represent negative ranks with their unsigned input types.
  R accepts numeric vectors representing exact nonnegative integers up to
  `2^53`, to avoid silently accepting precision-lost integers.
- The statistic depends on the evaluation cut-off. If a relevant result
  exists beyond the retrieved list, its rank is recorded as zero here, not
  as the position it would have in the full corpus.
- **Queries with no relevant result** are included as zero, rather than
  dropped from the denominator; this choice must be kept consistent across
  evaluations.

## Complexity

**O(q)** time, **O(1)** auxiliary memory in all four languages.

## Implementations

| Language | Folder |
| --- | --- |
| C | [mean_reciprocal_rank_c/](mean_reciprocal_rank_c/) |
| Python | [mean_reciprocal_rank_python/](mean_reciprocal_rank_python/) |
| Rust | [mean_reciprocal_rank_rust/](mean_reciprocal_rank_rust/) |
| R | [mean_reciprocal_rank_r/](mean_reciprocal_rank_r/) |

A CUDA implementation is not provided because GPU startup/transfer costs
are unlikely to help a small evaluation set; meaningful batching should be
studied separately.

## Examples and tests

```text
mrr([1, 2, 0, 4]) -> 0.4375
mrr([0, 0, 0])    -> 0
mrr([1])          -> 1
mrr([])           -> error
```

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

This function accepts **first relevant ranks** only; it does not process
document IDs, multiple relevance labels or graded gains. It cannot compute
mean average precision, NDCG or robust confidence intervals. Real benchmark
results require documented relevance judgments, cutoff policies and query
selection.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
