# No-Repeat N-gram Decoding Mask

## Why it belongs here

Autoregressive LLM decoders sometimes repeat phrases. No-repeat n-gram
constraints mask tokens that would recreate an n-gram seen in the generated
prefix; generation frameworks expose this as a configurable constraint
(Hugging Face, n.d.). This implementation computes a **boolean mask**:
a true entry means the corresponding next-token ID is forbidden.

## Definition

Given a history `h` of length `L` and n-gram width `n`, a candidate
token `t` is blocked if the concatenation of the last `n-1`
history tokens and `t` matches some *complete* n-gram already in `h`.
For `n=1`, all previously seen token IDs are blocked. If `L<n`,
there are no completed n-grams yet, so no token is blocked.

Example: `history=[1,2,3,1,2]`, `vocabulary_size=5`, `n=3` gives
`[false,false,false,true,false]` (token 3 would repeat `[1,2,3]`).

## Contract

- History is a sequence of **0..16,384** integer token IDs in
  `[0,vocabulary_size-1]`, with `1<=vocabulary_size<=4096`.
  `n` must be an integer in `[1,32]`.
- Empty histories and `n>length(history)` produce an all-false mask.
- Output is a boolean mask of length `vocabulary_size`, indexed by
  zero-based token ID (R vector element `id+1`).
- Invalid ranges, non-integer token IDs, non-finite values or malformed
  inputs are rejected. Python explicitly rejects booleans as IDs.
- C uses caller-owned output memory and leaves it unchanged if validation
  fails. The output must not overlap the history; a NULL history is
  accepted only when length is zero. Rust returns `Result<Vec<bool>,&str>`.
  Python and R raise errors on invalid input.
- Does **not** modify logits or select a token; callers may use the mask
  to exclude candidates before sampling or beam selection.

## Complexity

Direct reference scan: **O(L*n + V)** time and **O(V)** auxiliary space
for the mask, where `V` is vocabulary size. Implementations use no
hash map or external library. Suitable for learning, not optimized
for long sequences and large vocabularies.

## Implementations

| Language | Directory |
| --- | --- |
| C | [no_repeat_ngram_c](no_repeat_ngram_c/) |
| Python | [no_repeat_ngram_python](no_repeat_ngram_python/) |
| Rust | [no_repeat_ngram_rust](no_repeat_ngram_rust/) |
| R | [no_repeat_ngram_r](no_repeat_ngram_r/) |

Run: `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover unigrams, bigrams, trigrams, overlapping and repeated
prefixes, empty/short histories, and invalid parameter and token IDs.

## Limitations

This is a **hard** constraint: the mask can eliminate every available
candidate. It does not guarantee fluent or factual output, or
prevent semantic repetition with different tokens. Tokenization
is outside this module, and no efficiency benchmarking is claimed.

## References

- Hugging Face. (n.d.). *Generation*. https://huggingface.co/docs/transformers/main_classes/text_generation
