# Sparsemax Probability Transformation

## Why it belongs in this repository

Sparsemax maps neural-network logits to the probability simplex while allowing
**exact zeros**. Unlike softmax, it can assign no probability to irrelevant
classes or attention positions (Martins & Astudillo, 2016). This standalone
implementation illustrates simplex projection; it does not implement training.

## What it does

Given finite logits `x`, sparsemax is the Euclidean projection onto
`{p: p_i >= 0, sum(p_i) = 1}`:

```text
z = x - max(x)                 # translation leaves the projection unchanged
s = sorted(z, descending)
k = max{j: 1 + j*s[j] > sum(s[1..j])}
tau = (sum(s[1..k]) - 1) / k
sparsemax(x)[i] = max(z[i] - tau, 0)
```

The descending support criterion and threshold follow Martins and Astudillo
(2016). The subtraction of the maximum helps with large logits and preserves
the exact result mathematically.

For `[0, 0, -1]`, the output is `[0.5, 0.5, 0]`. For
`[3, 1, 0]`, the output is `[1, 0, 0]`. Equal logits return
uniform probabilities.

## Contract

- Input: a **nonempty**, finite real-valued vector of logits.
- Output: a vector of the same length, with nonnegative values summing to
  approximately 1; some values may be exactly 0.
- Empty, nonnumeric or non-finite input is invalid. Python/R raise errors,
  Rust returns `Err`, C returns `false`.
- C requires valid input and output buffers for `n` doubles. Invalid input
  does not modify output; exact in-place operation is supported.
- IEEE-754 rounding can affect values near the active-set boundary.
  Extremely disparate finite logits may produce negative-infinite shifted
  differences through overflow; those entries receive probability zero.
- This is a **forward vector transform**, not a batched layer, loss,
  gradient implementation or mask-aware attention operator.

## Complexity

Sorting dominates: **O(n log n)** time, **O(n)** auxiliary memory in all
implementations (plus a caller-owned output buffer in C).

## Implementations

| Language | Folder |
| --- | --- |
| C | [sparsemax_c](sparsemax_c/) |
| Python | [sparsemax_python](sparsemax_python/) |
| Rust | [sparsemax_rust](sparsemax_rust/) |
| R | [sparsemax_r](sparsemax_r/) |

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Reference tests cover ties, sparse support, translation, singleton vectors,
extreme finite values, empty input, NaN, infinity and C in-place output.

## Limitations

No backward derivative, sparsemax loss, GPU kernel, batching, or benchmarks.
Dense sorting is not necessarily efficient for large vocabularies. Approximate
equality rather than bitwise agreement should be used across languages.

## References

Martins, A. F. T., & Astudillo, R. F. (2016). From softmax to sparsemax:
A sparse model of attention and multi-label classification. In
*Proceedings of the 33rd International Conference on Machine Learning*
(Vol. 48, pp. 1614–1623). Proceedings of Machine Learning Research.
https://proceedings.mlr.press/v48/martins16.html
