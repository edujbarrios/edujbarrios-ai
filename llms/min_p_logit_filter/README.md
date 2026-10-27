# Min-P Relative-Probability Filtering

## Why it belongs in LLM decoding

Min-p is a token filtering rule: rather than retaining a fixed count
(top-k) or a fixed cumulative mass (top-p), it keeps tokens whose
probability is sufficiently large *relative to the most probable
token* (Hugging Face, n.d.).

## Algorithm

Given finite next-token logits `s[i]` and `p` in `[0,1]`,
let `m=max(s)`. Since normalized softmax probabilities share the
same denominator, `P[i] >= p*max(P)` exactly when

```text
exp(s[i] - m) >= p
```

Compute these exponentials and return a boolean keep mask in
**original vocabulary order**. The highest-scoring token is always
kept, including when `p=1`; equally high tokens remain tied.

Examples:

- `s=[0,-1,-2]`, `p=0.3` -> `[true,true,false]`.
- `s=[5,5,0]`, `p=1` -> `[true,true,false]`.
- `p=0` keeps every token, even if `exp(s[i]-m)` underflows.

## Contract

- Vocabulary size `V=1..4096`; all logits finite in `[-1000,1000]`.
- `min_p` finite, in `[0,1]`. Invalid values are rejected.
- Python rejects booleans for any numeric parameter.
- C requires a separate caller-allocated `V`-element boolean output,
  which is not modified on invalid input and must not alias the logits.
  Rust returns `Result<Vec<bool>,&str>`, Python returns a bool
  list, and R a logical vector.
- The returned mask is not a normalized probability distribution
  and does not perform any sampling.

## Complexity

**O(V)** time and **O(V)** output space. C uses O(1) additional
scratch space. No external numerical libraries are needed.

## Implementations

| Language | Directory |
| --- | --- |
| C | [min_p_logit_filter_c](min_p_logit_filter_c/) |
| Python | [min_p_logit_filter_python](min_p_logit_filter_python/) |
| Rust | [min_p_logit_filter_rust](min_p_logit_filter_rust/) |
| R | [min_p_logit_filter_r](min_p_logit_filter_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover relative cutoffs, tied maxima, zero and unit thresholds,
large logit gaps, single-token vocabularies, and invalid input.

## Limitations

This is purely a next-token selection mask. It is not a generative
language model, a top-p nucleus rule, or a substitute for repetition
controls; sampling and renormalization remain the caller's job.
Production libraries may combine min-p with other processors in
different orders.

## References

- Hugging Face. (n.d.). *Generation*. https://huggingface.co/docs/transformers/main_classes/text_generation
