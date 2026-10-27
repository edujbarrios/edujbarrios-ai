# Sign-Aware Repetition Penalty

## Why it belongs here

Autoregressive language models can repeat tokens or short phrases when
decoding. A repetition penalty modifies the *logits* of previously
generated token IDs before the probability distribution is sampled.
A sign-aware form is used in common LLM decoding libraries
(Hugging Face, n.d.).

## Mathematical definition

For vocabulary logit `s[t]` and penalty `p>=1`:

```text
adjusted[t] = s[t] / p  if t appeared in history and s[t] > 0
adjusted[t] = s[t] * p  if t appeared in history and s[t] < 0
adjusted[t] = s[t]      otherwise
```

A token appearing multiple times is **penalized once**, not repeatedly.
For `logits=[2,-2,0,1]`, `history=[0,1,1,2]`, and `p=2`,
the result is `[1,-4,0,1]`.

The penalty cannot guarantee that a seen token will never reappear;
use the hard `no_repeat_ngram` mask for that different purpose.

## Contract

- Vocabulary contains `1..4096` finite real logits in
  `[-1000,1000]`; history has `0..16384` zero-based token IDs,
  each an integer in `[0,vocabulary_size-1]`.
- Penalty must be finite and in `[1,10]`. A penalty of 1 or an
  empty history leaves all logits unchanged.
- Python rejects bools as numbers/IDs. R rejects missing, nonfinite
  and fractional token IDs. The C API uses size_t token IDs and
  caller-allocated output (unmodified on failure); output must
  not alias input. Rust returns `Result<Vec<f64>, &str>`;
  Python returns a list and R returns a numeric vector.
- A negative zero is considered zero and is not multiplied. Valid
  inputs are bounded so the result remains representable as f64.

## Complexity

**O(V+L)** time and **O(V)** auxiliary memory for seen-token bookkeeping
and the returned output, where V is vocabulary size and L is history length.

## Implementations

| Language | Folder |
| --- | --- |
| C | [repetition_penalty_c](repetition_penalty_c/) |
| Python | [repetition_penalty_python](repetition_penalty_python/) |
| Rust | [repetition_penalty_rust](repetition_penalty_rust/) |
| R | [repetition_penalty_r](repetition_penalty_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from the repository root.

Tests include positive/negative/zero logits, duplicate and empty
histories, penalty 1, invalid IDs, nonfinite inputs and parameter limits.

## Limitations

This is a token-level heuristic, not a semantic repetition detector.
It does not generate tokens, modify logits for unseen tokens, or
specify the subsequent softmax/sampling procedure. A high penalty
can damage fluency and can still leave repeated tokens probable.

## References

- Hugging Face. (n.d.). *Generation*. https://huggingface.co/docs/transformers/main_classes/text_generation
