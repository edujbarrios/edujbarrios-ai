# Reciprocal Rank Fusion (RRF)

**Stage:** 04 — Information Retrieval · **Languages:** C, Python, Rust and R.

## Why this algorithm is here

AI retrieval systems often combine independent rankings, such as lexical
BM25 results and embedding-based nearest neighbors. RRF combines **rank
positions**, not incomparable scores, into one deterministic result list.
Cormack et al. (2009) introduced the method for information retrieval.
This block is a small, verifiable retrieval primitive; it is **not** a
complete search engine or ranking benchmark.

## Algorithm

For each item `d` appearing in ranked lists `A` or `B`:

```text
RRF(d) = sum(1 / (60 + rank_r(d))) for lists r containing d
```

Ranks start at **1**. The constant is fixed at **60** to keep all language
implementations equivalent. Results are sorted by decreasing RRF score,
then by increasing numeric item ID for ties.

```text
A = [10, 20, 30]
B = [30, 10, 40]
fuse(A, B) = [10, 30, 20, 40]
```

## Shared contract

- Input: two ranked lists of unique **nonnegative integer IDs** in
  `0..2,147,483,647`. An ID may appear once in **each** list, but not
  more than once in the *same* list.
- Either list may be empty, but not both. Order encodes rank; numerical
  relevance scores are not required.
- Output: every distinct input ID once, in deterministic fused order.
  Scores are not returned by this initial API.
- Reject duplicate IDs in a list, negative or out-of-range IDs, malformed
  input or two empty lists.
- Python/R raise errors, Rust returns `Result<Vec<i32>, ...>`. R returns
  an integer vector with **zero-based IDs** unchanged.
- C returns `bool`, accepts caller-owned output of capacity `len_a +
  len_b` and a distinct `size_t* out_count`. Neither output is
  modified on failure. Buffer validity and non-overlap are C caller
  obligations.

## Complexity

Let `L=len(A)+len(B)`, `U` be the number of distinct IDs.

- C: straightforward linear-score lookup, **O(L² + U log U)** time,
  O(U) scratch memory.
- Python: dict accumulation and sort, expected O(L + U log U) time,
  O(U) memory.
- Rust: hash-map accumulation and sort, expected O(L + U log U) time,
  O(U) memory.
- R: matching each candidate against the ranked lists,
  O(U L + U log U) time, O(U + L) memory.

These are reference implementations; optimized hash maps in C or
multi-ranking generalization can be future focused PRs.

## Implementations

| Language | Directory |
| --- | --- |
| C | [reciprocal_rank_fusion_c](reciprocal_rank_fusion_c/) |
| Python | [reciprocal_rank_fusion_python](reciprocal_rank_fusion_python/) |
| Rust | [reciprocal_rank_fusion_rust](reciprocal_rank_fusion_rust/) |
| R | [reciprocal_rank_fusion_r](reciprocal_rank_fusion_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover overlapping rankings, score ties, one empty ranking,
duplicate IDs, negative and invalid IDs, and both-empty input.

## Limitations

This combines **exactly two rankings** with a fixed fusion parameter.
It does not model lexical retrieval, vector indexing, learning-to-rank,
weighting of result sources or pagination. RRF is heuristic and is not
universally superior to calibrated score fusion on every dataset.

## References

Cormack, G. V., Clarke, C. L. A., & Büttcher, S. (2009). Reciprocal rank
fusion outperforms Condorcet and individual rank learning methods. In
*Proceedings of the 32nd International ACM SIGIR Conference on Research
and Development in Information Retrieval* (pp. 758–759).
Association for Computing Machinery.
https://doi.org/10.1145/1571941.1572114
