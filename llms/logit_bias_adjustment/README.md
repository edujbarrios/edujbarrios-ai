# Sparse Logit Bias Adjustment

## Motivation

A text generator can adjust selected vocabulary token scores *before*
probability normalization and sampling. A sparse logit bias map allows
callers to encourage or discourage individual token IDs without requiring
a full vocabulary-sized bias vector (OpenAI, n.d.).

## Algorithm

Given `V` original logits `s[v]`, `B` distinct zero-based token IDs
`ids[j]`, and corresponding additive biases `biases[j]`:

```text
adjusted = copy(logits)
for j in 0..B-1:
    adjusted[ids[j]] = logits[ids[j]] + biases[j]
```

Example: `logits=[1,2,3]`, `ids=[0,2]`, `biases=[-1,0.5]`
produces `[0,2,3.5]`. With empty IDs and biases, scores are unchanged.
Unlike a presence or frequency penalty, this method is independent of
generated history.

## Contract

- `V` is 1..4096 with finite logits in `[-1000,1000]`.
- `B` is 0..`V`; IDs and bias arrays have equal length. IDs must be
  **distinct integer token IDs** in `[0,V-1]`. Biases are finite real
  numbers in `[-100,100]`. This reference intentionally disallows duplicate
  IDs instead of giving duplicate entries an ambiguous ordering.
- Output: a `V`-element vector of finite biased logits.
- Invalid lengths, duplicate IDs, nonfinite or out-of-range numbers fail.
  C returns false and leaves output unchanged on failure. Python/R raise
  errors; Rust returns `Result<Vec<f64>, &str>`. Python rejects booleans
  as numbers and IDs.
- C requires a separate output buffer or an **exact** in-place alias with
  the logits; it supports either. All validation happens before writes.
- IDs are always **zero-based**, including in R.

## Complexity

Time **O(V+B)**, additional space **O(V)** for a presence bitmap
(excluding any newly allocated output). C's bitmap is fixed at 4096 bytes.

## Implementations

| Language | Directory |
| --- | --- |
| C | [logit_bias_adjustment_c](logit_bias_adjustment_c/) |
| Python | [logit_bias_adjustment_python](logit_bias_adjustment_python/) |
| Rust | [logit_bias_adjustment_rust](logit_bias_adjustment_rust/) |
| R | [logit_bias_adjustment_r](logit_bias_adjustment_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover positive/negative biases, empty maps, zero bias, exact C
in-place operation and duplicate, out-of-range and invalid parameters.

## Limitations

The output contains adjusted scores, not normalized probabilities. Caller
is responsible for tokenization, sampling or greedy selection, and special
token handling. Logit adjustment alone does not guarantee a specific next
token or mitigate unsafe model behavior.

## References

OpenAI. (n.d.). *Create chat completion*. OpenAI API reference.
https://platform.openai.com/docs/api-reference/chat/create
