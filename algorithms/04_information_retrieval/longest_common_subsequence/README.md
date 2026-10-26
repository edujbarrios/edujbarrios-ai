# Longest Common Subsequence

## Why it belongs in this repository

Longest common subsequence (LCS) measures sequence overlap while
preserving token order. It is a classical dynamic-programming building
block for text comparison, document alignment and diff-style matching
(Cormen et al., 2022). Unlike a longest common *substring*, the matched
tokens need not be adjacent.

## Algorithm

Given sequences A and B, the LCS length is defined recursively:

    L(i,0) = L(0,j) = 0
    L(i,j) = 1 + L(i-1,j-1)          when A[i-1] == B[j-1]
    L(i,j) = max(L(i-1,j), L(i,j-1)) otherwise

Only the previous and current dynamic-programming rows are required.
The shorter input is placed on the column axis to reduce memory usage.

## Contract

- Inputs: two sequences of signed 32-bit integer token IDs, in their
  original order, with duplicates allowed.
- Output: a nonnegative integer LCS length, at most min(n,m).
- Empty sequences are valid and return zero; equal sequences return
  their number of elements, including duplicates.
- Python validates integer IDs in [-2147483648,2147483647] and rejects
  booleans and noninteger values with ValueError.
- R accepts finite, whole-number numeric vectors within the same
  range. Rust uses i32 slices, and C uses int32_t arrays.
- C returns a success Boolean and writes its output only after success.
  Null data pointers are allowed only when corresponding lengths are zero.
  Output pointer must be nonnull; declared memory lengths must be valid.

## Complexity

O(nm) time and O(min(n,m)) auxiliary space for n and m input lengths.
There is no early-stop or subquadratic approximation.

## Implementations

| Language | Folder |
| --- | --- |
| C | [longest_common_subsequence_c](longest_common_subsequence_c/) |
| Python | [longest_common_subsequence_python](longest_common_subsequence_python/) |
| Rust | [longest_common_subsequence_rust](longest_common_subsequence_rust/) |
| R | [longest_common_subsequence_r](longest_common_subsequence_r/) |

## Examples and tests

    LCS([1,2,3,4], [2,4,3]) = 2
    LCS([1,1,2], [1,2,1])   = 2
    LCS([], [1,2])          = 0
    LCS([1,2], [1,2])       = 2

From the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This returns the *length*, not an actual matching token alignment.
It does not tokenize strings or support weighted substitutions.
Quadratic runtime can be expensive for long documents; token IDs must
be assigned consistently upstream.

## References

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022).
*Introduction to algorithms* (4th ed.). MIT Press.
