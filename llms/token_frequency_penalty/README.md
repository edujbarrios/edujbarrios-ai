# Token Frequency Penalty for Next-Token Logits

## LLM decoding

Frequency penalties discourage repetition during autoregressive
text generation. Unlike a presence penalty, which applies once
for a previously seen token, this method subtracts a penalty for
**every occurrence** in the generated history. Such a decoding
parameter is documented by the OpenAI API (OpenAI, n.d.).

## Algorithm

For a vocabulary of size `V`, original logit `s[v]`, history
`h` of zero-based token IDs, and nonnegative penalty `a`:

```text
count[v] = number of occurrences of token v in h
adjusted[v] = s[v] - a * count[v]
```

Examples:
- `s=[1,2,3]`, `h=[0,2,2]`, `a=0.5`
  gives `[0.5,2,2]`.
- `s=[-1,0,1]`, `h=[0,0]`, `a=1`
  gives `[-3,0,1]` (same arithmetic for negative scores).
- Empty history or zero penalty leaves the logits unchanged.

## Contract

- `V=1..4096` finite logits each in `[-1000,1000]`.
- History may contain `0..4096` integer token IDs, each
  in `[0,V-1]`. Duplicates are allowed and counted.
- Penalty is a finite scalar in `[0,5]`.
- Invalid length, nonfinite logits/penalty, and out-of-range
  history tokens are rejected. Python rejects bool numeric inputs.
- Return a flat `V`-element floating-point vector of adjusted
  logits. No softmax or sampling is performed.
- C accepts a separate caller-owned `V`-element output buffer
  which stays unchanged if validation fails. Rust returns
  `Result<Vec<f64>,&str>`; Python and R return numeric vectors.
  History IDs are zero-based in all four languages.

## Complexity

Building the histogram and applying the penalty takes
**O(V+H)** time and **O(V)** scratch for token counts,
where H is the number of historical tokens.

## Implementations

| Language | Source and tests |
| --- | --- |
| C | [token_frequency_penalty_c](token_frequency_penalty_c/) |
| Python | [token_frequency_penalty_python](token_frequency_penalty_python/) |
| Rust | [token_frequency_penalty_rust](token_frequency_penalty_rust/) |
| R | [token_frequency_penalty_r](token_frequency_penalty_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover repeated IDs, negative logits, empty histories,
zero penalty, invalid token IDs and invalid parameters.

## Limitations

This local score adjustment is not a training loss, a token
sampler, or the sign-aware repetition-penalty rule in the
separate `repetition_penalty` module. The penalty treats
every prior occurrence equally and does not distinguish
semantic or syntactic repetition.

## References

- OpenAI. (n.d.). *Create chat completion*. OpenAI API reference. https://platform.openai.com/docs/api-reference/chat/create
