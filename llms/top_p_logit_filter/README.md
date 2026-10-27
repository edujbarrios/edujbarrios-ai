# Nucleus (Top-P) Logit Filtering

## Why it matters for LLMs

Top-p (nucleus) decoding reduces the next-token candidate set to the
smallest number of high-probability tokens whose combined probability
mass reaches a chosen threshold (Holtzman et al., 2020). This module
returns a **keep mask**, not sampled text or modified logits.

## Algorithm

Given `V` logits, compute a numerically stable softmax:

```text
w[i] = exp(logit[i] - max(logits))
P[i] = w[i] / sum(w)
```

Sort token IDs by decreasing logit, breaking ties by the smaller
original zero-based ID. Retain the shortest **nonempty** prefix
whose cumulative `P` is **at least** `top_p`. For
`top_p = 1`, keep all tokens by definition.

Examples: `logits=[0,0,0,0]` with `top_p=0.6` keeps
`[true,true,true,false]`. For `[2,1,0]`, `top_p=0.5`
keeps the first token; `top_p=0.8` keeps the first two.

## Contract

- `1 <= V <= 4096`, all logits finite in `[-1000,1000]`.
- Finite `top_p` in `(0,1]`. Empty logits and malformed
  or out-of-range inputs are rejected.
- Ties are resolved by increasing original index. At least one
  token is always selected. The return is a V-element boolean
  mask indexed by token ID, not rank.
- Python rejects booleans for numeric parameters. C writes to
  a caller-owned V-element nonoverlapping output mask, unchanged
  if validation fails. Rust returns `Result<Vec<bool>,&str>`;
  Python and R return boolean/logical vectors.
- Exp underflow for very unlikely tokens is acceptable, and all
  probability calculations use double precision. The threshold
  is reached before the last retained token is emitted.

## Complexity

The C reference uses deterministic selection-sort ranking,
**O(V²)** time and **O(V)** scratch memory. Python, Rust and R
sort in **O(V log V)** time with **O(V)** space. All versions
also take O(V) for validation, softmax and prefix selection.

## Implementations

| Language | Directory |
| --- | --- |
| C | [top_p_logit_filter_c](top_p_logit_filter_c/) |
| Python | [top_p_logit_filter_python](top_p_logit_filter_python/) |
| Rust | [top_p_logit_filter_rust](top_p_logit_filter_rust/) |
| R | [top_p_logit_filter_r](top_p_logit_filter_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests exercise uniform and asymmetric logits, ties, extreme
logit differences, one-element vocabularies, `top_p=1`,
and invalid inputs.

## Limitations

This is a hard candidate filter, not a sampler. You must
renormalize surviving logits before actual token sampling.
Typical production implementations use vectorized kernels
and may apply minimum-token rules beyond this contract.
A small nucleus does not guarantee fluent or factual text.

## References

- Holtzman, A., Buys, J., Du, L., Forbes, M., & Choi, Y. (2020). *The curious case of neural text degeneration*. International Conference on Learning Representations. https://openreview.net/forum?id=rygGQyrFvH
