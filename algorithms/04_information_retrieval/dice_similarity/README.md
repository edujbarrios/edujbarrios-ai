# Sørensen–Dice Similarity

## Why it belongs in this repository

The Dice similarity coefficient measures set overlap and is useful for
comparing document token sets, shingles, and retrieval candidates. It is
closely related to Jaccard similarity but divides twice the overlap by the
sum of set sizes (Dice, 1945).

## Mathematical definition

For sets A and B:

    Dice(A, B) = 2 * |A intersection B| / (|A| + |B|)

The result is in [0, 1]. Here Dice(empty, empty) = 1 by explicit convention;
an empty set compared with a nonempty set has score 0. Input sequences are
treated as sets, so duplicate IDs do not contribute multiple times.

## Contract

- Inputs: two sequences of signed 32-bit integer token IDs, including
  negative IDs and duplicates. Values must be in [-2147483648, 2147483647].
- Output: a finite scalar in [0, 1].
- Both empty inputs return 1; one empty input returns 0. Identical
  nonempty sets return 1 regardless of order or duplicate IDs.
- Python rejects Boolean, noninteger and out-of-range IDs with ValueError.
  R accepts finite, whole-number numeric IDs in the signed 32-bit range.
  C uses int32_t arrays; Rust uses i32 slices.
- C returns a success Boolean and does not modify the output on failure.
  Null input pointers are allowed only for length zero; output pointer
  must be nonnull. C pointers must reference their declared lengths.

## Complexity

Sort, deduplicate and intersect each input: O(n log n + m log m) time and
O(n + m) auxiliary storage, with n and m the original input lengths.
This implements exact set similarity; no approximate hashing or string
tokenization is performed.

## Implementations

| Language | Folder |
| --- | --- |
| C | [dice_similarity_c](dice_similarity_c/) |
| Python | [dice_similarity_python](dice_similarity_python/) |
| Rust | [dice_similarity_rust](dice_similarity_rust/) |
| R | [dice_similarity_r](dice_similarity_r/) |

## Examples and tests

    Dice([1, 1, 2, 3], [2, 3, 4]) = 2 / 3
    Dice([], [])                 = 1
    Dice([], [7])                = 0
    Dice([-5, 3], [3, -5])       = 1

From the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

Tests also cover invalid values and exact signed 32-bit boundary IDs.

## Limitations

This compares sets of integer token IDs, not raw text, multisets, fuzzy
strings, or continuous feature vectors. Upstream tokenization must assign
the same ID to the same token. The empty-set convention is a design choice
that should be documented in downstream use.

## References

Dice, L. R. (1945). Measures of the amount of ecologic association between
species. *Ecology, 26*(3), 297–302. https://doi.org/10.2307/1932409
