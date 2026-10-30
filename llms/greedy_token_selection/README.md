# Greedy Next-Token Selection

## Why it belongs in this repository

An autoregressive language model produces a score (logit) for each vocabulary
token. Greedy decoding chooses the highest-scoring next token at each step,
rather than drawing samples from a distribution. This is a simple,
deterministic baseline for studying generation strategies (Hugging Face,
n.d.).

## Algorithm

For nonempty, finite logits `s[0..V-1]`:

```text
next_token_id = smallest i with s[i] == max(s)
```

Tie-breaking by the smallest **zero-based** token index makes the result
reproducible across languages. For `[2,5,5,-1]`, the selected token ID
is `1`. For `[-4,-2,-3]`, it is `1`.

## Contract

- Input: a **nonempty** vector of finite real logits, one score per token.
- Output: the **zero-based index** of the maximum logit. On ties, select the
  first index.
- Reject empty, nonnumeric or nonfinite input. Python/R raise an error,
  Rust returns `Err` and C returns `false` without touching output.
- R returns an integer token ID, adjusted from R's 1-based indexing by
  subtracting 1. C writes to a caller-provided `size_t*`.
- The full vocabulary scores must be provided by the caller; no tokenizer,
  probability normalization or random number generation is used.

## Complexity

**O(V)** time and **O(1)** auxiliary space, single linear pass.

## Implementations

| Language | Folder |
| --- | --- |
| C | [greedy_token_selection_c](greedy_token_selection_c/) |
| Python | [greedy_token_selection_python](greedy_token_selection_python/) |
| Rust | [greedy_token_selection_rust](greedy_token_selection_rust/) |
| R | [greedy_token_selection_r](greedy_token_selection_r/) |

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover equal highest scores, all-negative logits, singleton input, large
finite magnitudes, and empty/nonfinite/invalid inputs.

## Limitations

Greedy selection at one time step is not a complete decoding loop and does
not enforce end-of-sequence stopping, constraints, temperature, repetition
penalties or beam search. It cannot explore alternative tokens after a
locally optimal choice. Unlike sampling, it cannot generate different outputs
from identical logits.

## References

Hugging Face. (n.d.). *Generation*. https://huggingface.co/docs/transformers/main_classes/text_generation
