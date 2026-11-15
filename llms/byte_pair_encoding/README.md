# Deterministic Byte-Level Byte Pair Encoding

## Why it belongs in AI

Many large language models represent text with subword tokens rather than
individual characters or whole words. Byte pair encoding (BPE) repeatedly
merges frequent adjacent symbols into larger units. BPE was adapted for
rare-word neural machine translation by Sennrich et al. (2016).
This example starts from **UTF-8 bytes** instead of characters, making it
able to represent arbitrary valid Unicode text without unknown tokens.

## Algorithm

1. Encode every corpus document separately as UTF-8 bytes, using token
   IDs `0..255`. **Never count pairs across document boundaries.**
2. Count adjacent token pairs in every document. Choose the pair with
   the **highest frequency**, breaking ties by the lexicographically
   smallest integer pair.
3. Stop when the highest frequency is below `min_frequency`
   (default `2`), or `max_merges` have been learned.
4. Assign the chosen pair the next token ID, starting with `256`;
   replace **nonoverlapping left-to-right** occurrences of that pair
   in each document.
5. Repeat, returning the ordered list of learned merges and final
   token IDs for every training document.

To tokenize new text, run `apply_byte_pair_merges(text, merges)`,
applying the learned pairs in the **original training order**.
Each recorded pair `[left, right]` implicitly creates token
ID `256 + its_position_in_merges`.

## Contract

- `train_byte_pair_encoding(corpus, max_merges, min_frequency=2)`
  accepts a list/tuple of **1..1000** strings containing a total of
  **1..100000 UTF-8 bytes**. Individual empty documents are allowed.
- `max_merges` is an integer `0..256`; `min_frequency` is an
  integer `2..100000`. Booleans are rejected.
- Returns `{"merges": [[left_id, right_id], ...],
  "token_ids": [[id, ...], ...]}`. Results are deterministic and
  the original corpus is never changed.
- `apply_byte_pair_merges(text, merges)` accepts one string of at
  most **100000 UTF-8 bytes**, including empty text; the provided
  merge list is validated against the sequential token-ID convention.
- Unsupported types, oversized inputs, malformed merges and invalid
  Unicode (such as unpaired surrogate characters) raise `ValueError`.

## Worked example

```python
trained = train_byte_pair_encoding(["abab", "abab"], max_merges=2)
# trained["merges"] == [[97, 98], [256, 256]]
# trained["token_ids"] == [[257], [257]]
apply_byte_pair_merges("abab", trained["merges"])
# [257]
```

The first merge makes `ab` a token (256), and the next merges
two adjacent `ab` tokens (257).

## Complexity

For `N` input bytes and `M` learned merges, the teaching
implementation recomputes pair frequencies and replaces matches in
**O(MN)** expected time (hash-table operations), and uses
**O(N + M)** space including the tokenized corpus and merge table.
Applying an existing merge list costs O(MN) time.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [byte_pair_encoding_python](byte_pair_encoding_python/) |

Other languages are not yet implemented.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover merge order, frequency ties, overlapping pairs, document
boundaries, Unicode input, zero merges, round-trip consistency between
training and applying merges, and strict input validation.

## Limitations

This is a **minimal byte-level BPE trainer**, not a full GPT-style
tokenizer. There are no word-boundary rules, pretokenization,
normalization, reserved/special tokens, vocabulary serialization,
or regex splitting. A small synthetic corpus can give unhelpful merges.
Pair frequencies count overlapping occurrences but replacements are
left-to-right nonoverlapping. The output token IDs are not compatible
with any pretrained model's vocabulary.

## References

Sennrich, R., Haddow, B., & Birch, A. (2016). Neural machine translation
of rare words with subword units. In *Proceedings of the 54th Annual
Meeting of the Association for Computational Linguistics (Volume 1:
Long Papers)* (pp. 1715–1725).
https://doi.org/10.18653/v1/P16-1162
