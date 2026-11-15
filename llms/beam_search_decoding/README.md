# Prefix-Aware Beam Search Decoding

## Why it belongs in this repository

Language models predict the next token **conditioned on all preceding
tokens**. Beam search keeps several promising partial generations rather
than committing to the highest-scoring token immediately. This entry
demonstrates autoregressive sequence search with a model-supplied scoring
callback and early end-of-sequence stopping (Graves, 2012).

Unlike the existing **Markov beam search** example in AI Systems, the
next-token scores here may depend on the **entire generated prefix**, not
only its last state.

## Algorithm

Starting with the empty token prefix and cumulative log score zero, repeat:

1. For every unfinished hypothesis, call `next_log_scores(prefix)`
   to obtain one log score for each token ID `0..V-1`.
2. Expand each hypothesis by every unmasked token; add that token's log
   score to the hypothesis's cumulative score.
3. Preserve hypotheses that already ended in the optional EOS token
   without expanding or rescoring them.
4. Keep the best `beam_width` hypotheses, sorting by **descending**
   cumulative score and then **ascending lexicographic token sequence**
   to break ties reproducibly.

Stop after `max_new_tokens` expansions or when all retained beams are
finished. Return a ranked list of `(token_ids, cumulative_log_score)` pairs.

## Contract

- Python input: a callable `next_log_scores(prefix_tuple)`, vocabulary
  size `1..4096`, beam width `1..128`, and integer generation budget
  `0..256`. Optional `eos_token_id` must be a valid zero-based ID.
- The callback must return a list or tuple of **exactly V** real,
  nonpositive token log scores. `-inf` masks a token. `NaN`, `+inf`,
  positive scores, booleans and shape mismatches are invalid.
- The scores can be log probabilities from a language model, or
  unnormalized nonpositive log scores. The algorithm does not normalize
  them. Callbacks should be deterministic for reproducible output.
- An all-masked continuation is dropped; if no candidates remain, raise
  `ValueError`. Nonfinite cumulative sums also raise `ValueError`.
- An EOS-completed hypothesis is retained at its previous score and length.
  `max_new_tokens=0` returns `[([], 0.0)]` without invoking the callback.
- Only generated tokens are returned (no prompt tokens). Ties are resolved
  by token IDs rather than callback ordering.

## Worked example

At the empty prefix, scores for tokens `0` and `1` are `[-0.1, -0.2]`.
After token `0`, scores are `[-5, -5]`; after token `1`, they are
`[-0.01, -0.02]`. With beam width `2` and budget `2`, the two best
sequences are `[1,0]` (score `-0.21`) and `[1,1]` (score `-0.22`).
A width-one greedy search would instead commit to token `0` first.

## Complexity

With `T` expansions, beam width `B`, vocabulary `V`, sorting all
candidates costs **O(T B V log(B V))** time, not including the callback's
model computation. Candidate token tuples consume **O(B V T)** space.
This pedagogical implementation is not optimized for large vocabularies.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [beam_search_decoding_python](beam_search_decoding_python/) |

A Python-only callback-based reference is provided. Other language
implementations are not declared.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover prefix-dependent optimal paths, greedy-vs-beam differences,
EOS early stopping, tie ordering, masked tokens, zero-token generation,
invalid shapes and parameters, and cumulative-score overflow.

## Limitations

The algorithm does not supply a model, tokenizer, cached KV states,
length normalization or sampling. Raw cumulative log scores generally
favor short EOS-terminated hypotheses. A fixed beam may prune a future
optimal sequence. Callback exceptions are propagated, and callbacks
with side effects can make results nondeterministic.

## References

Graves, A. (2012). *Sequence transduction with recurrent neural networks*
(arXiv:1211.3711). arXiv. https://arxiv.org/abs/1211.3711
