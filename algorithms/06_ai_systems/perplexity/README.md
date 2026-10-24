# Language-Model Perplexity from Token Log-Probabilities

## Why it belongs in AI

**Perplexity** measures how well a probabilistic language model
predicts a sequence of observed tokens. It is the exponential of
the mean negative log-probability of the *reference (ground-truth)*
tokens. A smaller perplexity corresponds to assigning higher
probability to the observed sequence on average. It is widely
used in statistical language modeling (Chen & Goodman, 1999).

Perplexity is **not** the same as human-perceived generation quality
or factual accuracy. It is especially sensitive to the tokenizer,
evaluation data, and how conditionals are defined.

## Mathematics

Given natural-log conditional probabilities of the **actual**
observed tokens, `l[i] = ln P(token_i | prior context)`:

```text
mean_nll = - (1/n) * sum_i l[i]
perplexity = exp(mean_nll)
```

All log-probabilities must be **finite and <=0**. An input of exactly
0 represents a token predicted with probability 1; large negative
values represent very unlikely observed tokens. The implementation
calculates `mean_nll` directly from logs instead of multiplying
tiny probabilities that would underflow.

Examples:

```text
log_probs = [ln(0.5), ln(0.25)]
mean_nll = ln(sqrt(8))
perplexity = sqrt(8) ≈ 2.8284271247

log_probs = [0,0,0]
perplexity = 1

log_probs = [ln(0.25), ln(0.25)]
perplexity = 4
```

## Shared contract

- A **nonempty** vector of **1..4096** natural-log probabilities.
  Every element is a finite real `<=0`.
- Return one finite scalar `perplexity>=1`. Reject NaN, infinity,
  positive log-probabilities, invalid vector shape, and any
  unrepresentable mean negative log-likelihood or exponential
  result. **Do not silently saturate to infinity.**
- Probabilities of exactly zero have log probability `-Inf`,
  which is rejected here; evaluating an impossible observed token
  requires a different policy.
- No token IDs or language model are supplied: the **caller**
  computes conditional log-probabilities for ground-truth tokens,
  with a consistent tokenization and context convention.
- C writes caller-owned `double* result` only on success, never
  modifying it for invalid input; input and output must not alias.
  Rust returns `Result<f64,...>`; Python/R raise an error.
  Python rejects boolean numeric inputs.
- To reduce avoidable summation overflow, accumulate each
  `(-logp)/n` contribution, and only then call `exp`.
  With finite precision, tiny input differences can still affect
  the result. No arbitrary probability floor is imposed.

## Complexity

**O(n)** time and **O(1)** auxiliary space.

## Implementations

| Language | Directory |
| --- | --- |
| C | [perplexity_c](perplexity_c/) |
| Python | [perplexity_python](perplexity_python/) |
| Rust | [perplexity_rust](perplexity_rust/) |
| R | [perplexity_r](perplexity_r/) |

Tests (repository root):

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include exact-known probabilities, one-token and certain-token
cases, long repeated sequences, positive/invalid log-probabilities,
empty inputs and exponential overflow.

## Limitations

Token-level perplexity only; **not** sequence-level normalized
probability for variable-length texts, bits per byte, character
perplexity, masked-language-model evaluation or sampling diversity.
Comparisons between different vocabularies or tokenizers may be
misleading. No training loop, text pipeline, CUDA kernel or
hardware benchmark is provided.

## References

Chen, S. F., & Goodman, J. (1999). An empirical study of smoothing
techniques for language modeling. *Computer Speech & Language,
13*(4), 359–394. https://doi.org/10.1006/csla.1999.0128
