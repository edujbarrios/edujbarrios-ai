# Precision at K (P@K)

## Why it belongs in the AI hub

Retrieval-augmented generation, document search and recommendation systems
need evaluation metrics separate from the algorithms that rank candidates.
Precision at K reports what fraction of the first `k` retrieved results
are relevant under a binary relevance assessment. It is described in
standard information-retrieval evaluation (Manning et al., 2008).

## Mathematical definition

Given a ranked sequence `relevance` of binary relevance judgments:

```text
Precision@k = (sum_{i=1}^{k} relevance[i]) / k
```

Example: the top four judgments `[1, 0, 1, 1]` yield
`P@3 = 2/3` and `P@4 = 3/4`.

## Contract

- The input is a **nonempty**, ordered sequence of binary labels (only 0
  and 1); rank 1 is the first element. Scores are not accepted in place of
  relevance judgments.
- `k` is an integer with `1 <= k <= number_of_results`.
- The entire input, including judgments **after k**, must be valid.
  Invalid labels, empty inputs, or invalid `k` produce an error.
- Output is a finite floating-point scalar in `[0, 1]`. A run with no
  relevant results in its first `k` returns zero.
- C accepts a `uint8_t` array and an output pointer, leaving the output
  unchanged on error. The caller guarantees an array of at least `n`
  elements. Python/R raise errors. Rust returns `Result<f64, ...>`.
- This **does not** assume a total count of all relevant items in the
  corpus and cannot compute recall or average precision.

## Complexity

**O(n)** time to validate the entire ranking, plus O(k) arithmetic
within that scan; **O(1)** auxiliary space for all implementations.

## Implementations

| Language | Directory |
| --- | --- |
| C | [precision_at_k_c](precision_at_k_c/) |
| Python | [precision_at_k_python](precision_at_k_python/) |
| Rust | [precision_at_k_rust](precision_at_k_rust/) |
| R | [precision_at_k_r](precision_at_k_r/) |

CUDA would not be justified for a tiny single ranking; this PR contains
no GPU variant.

## Examples and tests

```text
precision_at_k([1, 0, 1, 1], 3)  -> 0.6666666666666666
precision_at_k([1, 0, 1, 1], 4)  -> 0.75
precision_at_k([0, 0], 2)        -> 0.0
precision_at_k([1, 0, 2], 2)     -> error (invalid label after k)
```

From the repository root, run `python3 scripts/run_tests.py --strict`.
The tests also cover nonintegral k, k=0, k larger than the ranking,
and empty input.

## Limitations

Binary relevance only, no graded gain (NDCG), no query aggregation (MAP),
no uncertainty estimates and no handling of unjudged documents. A metric
computed on incomplete human relevance labels may not reflect real user
satisfaction.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
