# Top-K Score Selection

## Why it belongs in the AI hub

Retrieval engines, rerankers and nearest-neighbor systems routinely retain
the best-scoring candidates from a larger collection. A deterministic
Top-K step makes downstream ranking reproducible across implementations
and provides a building block for exact retrieval. Ranking by scores is a
standard information-retrieval operation (Manning et al., 2008).

## Definition

Given a nonempty sequence of scores `s[0:n]` and integer
`1 <= k <= n`, return **zero-based indices** of the `k` highest scores,
ordered by:

1. Descending numeric score.
2. Ascending original index for equal scores.

For scores `[0.8, 0.9, 0.9, -1.0]`, `k=3` returns
`[1, 2, 0]`. The result contains indices rather than the score values,
preserving the identity of the ranked items.

## Contract

- Scores must be finite real values. NaN, ±infinity, unsupported types,
  empty input, invalid `k` and `k > n` are errors.
- Negative scores, zero scores and repeated equal scores are supported.
- The complete input is validated before ranking. Python/R raise errors.
  Rust returns `Result<Vec<usize>, ...>`; C returns `bool` and writes to
  a caller-provided index array **only on success**.
- C's caller guarantees at least `n` scores and `k` index slots, with
  non-overlapping input/output buffers.
- R returns explicitly **zero-based indices** so its output matches the
  other languages rather than ordinary one-based R indexing.
- This is a **full-sort reference implementation**, not an optimal heap or
  selection algorithm. Do not infer any latency or GPU performance claim.

## Complexity

**O(n log n)** time for the full sort and **O(n)** auxiliary space for
score-index pairs. Output size is **O(k)**.

## Implementations

| Language | Folder |
| --- | --- |
| C | [top_k_c/](top_k_c/) |
| Python | [top_k_python/](top_k_python/) |
| Rust | [top_k_rust/](top_k_rust/) |
| R | [top_k_r/](top_k_r/) |

CUDA is not included; GPU top-k algorithms need a different parallel
selection strategy and device tests.

## Examples and tests

```text
top_k([0.8, 0.9, 0.9, -1.0], 3) -> [1, 2, 0]
top_k([1, 1, 1], 2)            -> [0, 1]
top_k([2, -4], 2)              -> [0, 1]
```

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` at the repository root.

## Limitations

No streaming updates, heap-based `O(n log k)` selection, GPU kernels,
approximate recall, score calibration or deduplication of documents.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
