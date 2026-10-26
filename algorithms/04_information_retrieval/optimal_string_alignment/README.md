# Optimal String Alignment Distance

## Why it belongs in this repository

Optimal string alignment (OSA), also called restricted Damerau–Levenshtein
distance, adds **adjacent transposition** to insert, delete, and substitute
edits. Such edits are useful for spelling correction and comparing sequences
of token IDs (Damerau, 1964). It complements this repository's Levenshtein
distance and longest common subsequence algorithms.

## Mathematical definition

Let D[i,j] be the minimum number of permitted edits transforming the first
i elements of A into the first j elements of B:

    D[i,0] = i; D[0,j] = j
    D[i,j] = min(D[i-1,j] + 1, D[i,j-1] + 1,
                 D[i-1,j-1] + (A[i-1] != B[j-1]))

Additionally, if i,j >= 2 and A[i-1] == B[j-2] and A[i-2] == B[j-1]:

    D[i,j] = min(D[i,j], D[i-2,j-2] + 1)

Each source position can participate in at most one edit. This is the
*restricted* OSA distance, **not** unrestricted Damerau–Levenshtein.
For example, OSA("CA","ABC") = 3, while unrestricted Damerau distance = 2.

## Contract

- Inputs: two ordered sequences of signed 32-bit integer token IDs.
  Duplicate IDs are allowed, and order matters. Empty sequences are valid.
- Output: a nonnegative integral edit distance.
- Insertions, deletions, substitutions, and swaps of adjacent elements
  each cost one. Both empty sequences return zero.
- Python accepts lists/tuples, rejecting Boolean, non-integer and out-of-range
  IDs with ValueError. R accepts one-dimensional vectors of whole finite
  signed 32-bit numeric IDs. C accepts int32_t arrays and Rust i32 slices.
- C returns a Boolean success flag, does not modify output on failure, and
  allows a null input pointer only for a zero-length input. Buffer lengths
  must be correct; the output pointer must not be null.

## Complexity

O(n*m) time and O(min(n,m)) auxiliary space, using three dynamic-programming
rows. The algorithm exchanges the arguments if needed to use fewer columns.

## Implementations

| Language | Folder |
| --- | --- |
| C | [optimal_string_alignment_c](optimal_string_alignment_c/) |
| Python | [optimal_string_alignment_python](optimal_string_alignment_python/) |
| Rust | [optimal_string_alignment_rust](optimal_string_alignment_rust/) |
| R | [optimal_string_alignment_r](optimal_string_alignment_r/) |

## Examples and tests

    OSA([1, 2], [2, 1])                   = 1
    OSA([1, 2, 3], [1, 3, 2])             = 1
    OSA([3, 1], [1, 2, 3])                = 3
    OSA([], [1, 2])                      = 2

The tests also verify invalid ID values, duplicates, substitutions,
deletions, and edge cases.

Run from the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

OSA is **not equal to the true Damerau–Levenshtein metric** in all cases:
a token may not be edited more than once. This algorithm returns only the
distance, not the edit script; it does not tokenize text, assign token IDs,
or implement Unicode grapheme semantics.

## References

Damerau, F. J. (1964). A technique for computer detection and correction
of spelling errors. *Communications of the ACM, 7*(3), 171–176.
https://doi.org/10.1145/363958.363994
