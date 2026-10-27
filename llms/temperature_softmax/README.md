# Temperature-Scaled Softmax

## LLM decoding context

Autoregressive language models predict scores (logits) for each possible
next token. **Temperature scaling** alters the sharpness of the resulting
sampling distribution: smaller temperatures concentrate probability on
higher-scoring tokens, while larger temperatures distribute it more
evenly (Hugging Face, n.d.). This module computes probabilities only;
it does not randomly sample tokens.

## Mathematical definition

For logits `x[i]` and temperature `T>0`:

```text
m = max(x)
w[i] = exp((x[i] - m) / T)
p[i] = w[i] / sum_j w[j]
```

Subtracting the maximum before exponentiation prevents overflow.
For logits `[0,2]` and `T=2`, the expected probabilities are
`[1/(1+e), e/(1+e)] ≈ [0.2689414, 0.7310586]`.
Uniform logits always give a uniform distribution. Adding an equal
constant to every logit leaves the probabilities unchanged.

## Input/output contract

- Input is a flat sequence of **1 to 4096** finite logits, each
  in the interval `[-1000,1000]`.
- Temperature must be finite and satisfy **0.01 ≤ T ≤ 100**.
  Out-of-range or invalid inputs produce an error; Python
  rejects booleans as numeric values.
- Output has the same length and consists of probabilities between
  0 and 1, summing to approximately 1 (rounding and underflow apply).
  An extremely unlikely token may receive exactly zero due to
  floating-point underflow.
- C writes to a separate caller-owned output buffer containing
  `length` doubles, unchanged on validation failure.
  Input/output aliasing is unsupported.
- Python returns a list, Rust a `Result<Vec<f64>,&str>`,
  and R a numeric vector.

## Complexity

**O(V)** time and **O(V)** output storage, where V is the
vocabulary size. C uses O(1) additional scratch memory.

## Implementations

| Language | Directory |
| --- | --- |
| C | [temperature_softmax_c](temperature_softmax_c/) |
| Python | [temperature_softmax_python](temperature_softmax_python/) |
| Rust | [temperature_softmax_rust](temperature_softmax_rust/) |
| R | [temperature_softmax_r](temperature_softmax_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Reference tests check uniform scores, temperature scaling,
shift invariance, stability with extreme logits, singleton
vocabularies and invalid parameters.

## Limitations

This is a deterministic distribution transform, not a tokenizer,
language model, top-p/top-k filter, entropy controller or
sampling algorithm. The temperature range is deliberately bounded
for educational reproducibility.

## References

- Hugging Face. (n.d.). *Generation*. https://huggingface.co/docs/transformers/main_classes/text_generation
