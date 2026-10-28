# Locally Typical Next-Token Filtering

## LLM context

Locally typical sampling seeks next-token candidates whose information
content is close to the model distribution's entropy, rather than simply
choosing the most probable tokens (Meister et al., 2023).
This module computes a **boolean keep mask**; it does not sample.

## Algorithm

Given logits `s[i]` and threshold `p`:

```text
m = max(s)
w[i] = exp(s[i] - m)
P[i] = w[i] / sum(w)
logZ = log(sum(w))
surprisal[i] = logZ - (s[i]-m)
entropy = sum_i P[i]*surprisal[i]
typicality[i] = abs(surprisal[i] - entropy)
```

Sort token IDs by *increasing* typicality and break ties by
*smaller original zero-based ID*. Keep the **shortest nonempty**
prefix whose cumulative probability is at least `p`.
For `p=1` keep all tokens. Output is in original ID order.

Computing surprisal from the stable logits rather than
`-log(P[i])` makes this work even when an extremely small
probability underflows to zero.

Examples: uniform logits `[0,0,0,0]` with `p=.6`
keep `[true,true,true,false]`; logits `[2,1,0]`
with `p=.8` keep the first two tokens.

## Contract

- `1<=V<=4096` finite logits in `[-1000,1000]`.
- `p` is finite in `(0,1]`; invalid inputs are rejected.
- C writes a distinct caller-owned V-element boolean mask,
  unchanged on validation failure. Rust returns
  `Result<Vec<bool>,&str>`; Python and R return boolean masks.
  Python rejects boolean numeric values.
- A one-token vocabulary always keeps that token. Equal scores
  resolve in original index order.
- Probability underflow of an extremely unlikely token is
  acceptable; all computations use double precision.

## Complexity

The C implementation uses deterministic selection sorting,
**O(V²)** time and O(V) scratch; Python, Rust and R use
**O(V log V)** sorting and O(V) auxiliary memory.

## Implementations

| Language | Directory |
| --- | --- |
| C | [typical_sampling_filter_c](typical_sampling_filter_c/) |
| Python | [typical_sampling_filter_python](typical_sampling_filter_python/) |
| Rust | [typical_sampling_filter_rust](typical_sampling_filter_rust/) |
| R | [typical_sampling_filter_r](typical_sampling_filter_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover uniform logits, asymmetric scores, one-element
inputs, zero-probability underflow, p=1 and invalid inputs.

## Limitations

This does not draw a sample or renormalize the surviving logits.
Typicality is computed on the unfiltered distribution; combining
it with top-k, top-p or repetition processors changes results
depending on processor order.

## References

- Meister, C., Pimentel, T., Wiher, G., & Cotterell, R. (2023). *Locally typical sampling*. Transactions of the Association for Computational Linguistics. https://arxiv.org/abs/2202.00666
