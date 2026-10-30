# Token Presence Penalty for Next-Token Logits

## Why it belongs here

Presence penalties discourage a language model from reusing a token that
already appeared in its generation history. Unlike a *frequency penalty*,
which grows with each occurrence, presence penalizes a token only **once**
regardless of how often it appeared. This decoding option is documented in
the OpenAI API reference (OpenAI, n.d.).

## Algorithm

Given `V` logits `s`, zero-based history token IDs `h`, and
nonnegative scalar penalty `alpha`:

```text
present[v] = 1 if token v appears at least once in h, otherwise 0
adjusted[v] = s[v] - alpha * present[v]
```

For logits `[1,2,3]`, history `[0,2,2]`, and penalty `0.5`,
the output is `[0.5,2,2.5]`, **not** `[0.5,2,2]`.
For empty history or penalty zero, logits remain unchanged.

## Contract

- Input: `V=1..4096` finite logits in `[-1000,1000]`; `0..4096`
  history IDs, each an integer in `[0,V-1]`; and a finite real penalty
  `alpha` in `[0,5]`.
- Output: an adjusted vector with `V` finite scores; identical token IDs
  in the history count only once.
- Invalid lengths, out-of-range or noninteger IDs, nonfinite values or
  invalid penalties are rejected.
- C uses separate caller-owned output with at least `V` doubles,
  unchanged on failure (exact in-place output is supported).
  Rust returns `Result<Vec<f64>, &str>`. Python/R raise errors.
  Python rejects bool values as logits, token IDs and penalties.
- History IDs are **zero-based** even in R. No softmax, tokenization or
  sampling is performed.

## Complexity

Time **O(V+H)**, auxiliary space **O(V)** for a presence bitmap,
where `H` is history length.

## Implementations

| Language | Folder |
| --- | --- |
| C | [token_presence_penalty_c](token_presence_penalty_c/) |
| Python | [token_presence_penalty_python](token_presence_penalty_python/) |
| Rust | [token_presence_penalty_rust](token_presence_penalty_rust/) |
| R | [token_presence_penalty_r](token_presence_penalty_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include duplicate history IDs, negative logits, zero penalty,
empty history, invalid IDs, invalid score ranges and C in-place output.

## Limitations

This operation adjusts logits but does not select tokens, normalize
probabilities, handle semantic repetition, or distinguish prompt tokens
from generated tokens. It penalizes every previously present token equally
and does not guarantee avoidance of repeats.

## References

OpenAI. (n.d.). *Create chat completion*. OpenAI API reference.
https://platform.openai.com/docs/api-reference/chat/create
