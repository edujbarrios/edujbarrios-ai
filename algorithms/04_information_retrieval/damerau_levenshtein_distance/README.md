# Unrestricted Damerau–Levenshtein Distance

## Why it belongs in this repository

Spelling correction and fuzzy matching often need insertions, deletions, substitutions and adjacent swaps. Unrestricted Damerau–Levenshtein distance allows a token to participate in multiple edits, unlike the restricted optimal string alignment (OSA) algorithm already included in this catalogue (Damerau, 1964).

## What it does

It computes the minimum number of unit-cost insertions, deletions, substitutions and transpositions of adjacent byte tokens to transform one sequence into another. A dynamic program remembers the most recent position of each of the 256 token values. An extra sentinel row and column allow a transposition to combine with intervening insertion/deletion edits without imposing OSA's single-use restriction.

For example:

    full_DL([67, 65], [65, 66, 67]) = 2
    restricted_OSA([67, 65], [65, 66, 67]) = 3

The first sequence represents CA and the second ABC; moving C past A, then inserting B, costs two edits.

## Contract

- Two ordered byte-token sequences, each of length 0..256; token IDs are integers in [0, 255], including zero.
- Output is a nonnegative integer edit distance. Empty versus empty is zero; empty versus nonempty has distance equal to the nonempty length.
- C accepts uint8_t buffers, including null pointers only when the corresponding length is zero. It returns false on invalid lengths/pointers/allocation and never changes the output on failure.
- Python accepts lists or tuples of exact byte-range integers (not bool) and raises ValueError for invalid tokens, types or lengths.
- Rust accepts u8 slices and returns None when either exceeds length 256; it returns Some(distance) otherwise.
- R accepts one-dimensional finite whole-number vectors of byte IDs and raises an error on malformed inputs.
- No input mutation. All edit operations have cost one.

## Complexity

O(n*m) time, O(n*m + 256) space, using the unrestricted dynamic-programming recurrence and a fixed-size table of last-seen token positions. For n or m equal to zero, the matrix setup is still valid.

## Implementations

| Language | Folder |
| --- | --- |
| C | [damerau_levenshtein_distance_c](damerau_levenshtein_distance_c/) |
| Python | [damerau_levenshtein_distance_python](damerau_levenshtein_distance_python/) |
| Rust | [damerau_levenshtein_distance_rust](damerau_levenshtein_distance_rust/) |
| R | [damerau_levenshtein_distance_r](damerau_levenshtein_distance_r/) |

## Examples and tests

    [1, 2] vs [2, 1]         = 1
    [67, 65] vs [65, 66, 67] = 2
    [1, 1, 2] vs [1, 2, 1]   = 1
    [] vs [0, 255]          = 2

Tests cover the distinction from OSA, repeated tokens, zero and 255 byte values, symmetric distances, maximum-length inputs and validation failures. From the root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This accepts already-tokenized byte IDs; it does not parse Unicode grapheme clusters, normalize text, recover an edit script or use weighted edit costs. Its O(n*m) memory and length limit are intentional for educational clarity, not an optimized library.

## References

Damerau, F. J. (1964). A technique for computer detection and correction of spelling errors. *Communications of the ACM, 7*(3), 171–176. https://doi.org/10.1145/363958.363994
