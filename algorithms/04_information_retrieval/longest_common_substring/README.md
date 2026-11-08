# Longest Common Substring (Contiguous, ASCII)

## Why it belongs in this repository

Longest common **substring** identifies a contiguous shared span between two strings, useful in lexical matching, plagiarism hints, and comparing similar text fragments. It differs from **longest common subsequence**, which permits skipped characters (Gusfield, 1997).

## Algorithm

For strings `a` and `b`, dynamic programming tracks the length `D[i,j]` of the longest common **suffix** ending at positions `i-1` and `j-1`:

```text
D[i,j] = D[i-1,j-1] + 1   when a[i-1] == b[j-1]
D[i,j] = 0                otherwise
```

Find the maximum `D[i,j]`; its start positions are `i - D[i,j]` and `j - D[i,j]`. Keep only the previous and current DP rows. On equal maximum lengths, choose **the smallest start index in a**, then **the smallest start index in b**.

## Contract

- Both inputs are printable ASCII strings (U+0020–U+007E), each of length **0..512**. Comparison is exact and case-sensitive. Reject Unicode, control characters, oversize strings, and invalid types in Python/R.
- Return `(length, start_a, start_b)` with **zero-based offsets**. For no common character (including either input empty), return `(0,0,0)`. The longest substring itself can be extracted as `a[start_a:start_a+length]`.
- C accepts NUL-terminated strings, plus **three distinct non-NULL** `size_t *` output pointers. It returns `bool`, validates first, and leaves all three outputs unchanged on failure. Embedded NUL cannot be expressed within a C string.
- Rust returns `Result<(usize, usize, usize), &str>`; Python returns `tuple[int,int,int]`; R returns `list(length=integer, start_a=integer, start_b=integer)`.
- Ties use the same rule in all four languages, regardless of iteration order.

## Complexity

**O(nm)** time and **O(m)** auxiliary memory, where `n = len(a)`, `m = len(b)`. Uses two DP rows instead of an entire 2D matrix.

## Implementations

| Language | Folder |
| --- | --- |
| C | [longest_common_substring_c](longest_common_substring_c/) |
| Python | [longest_common_substring_python](longest_common_substring_python/) |
| Rust | [longest_common_substring_rust](longest_common_substring_rust/) |
| R | [longest_common_substring_r](longest_common_substring_r/) |

## Examples and tests

```text
match("banana", "ananas")  = (5, 1, 0)  # "anana"
match("abcdef", "zabcf")   = (3, 0, 1)  # "abc"
match("ababa", "babab")    = (4, 0, 1)  # tie broken by earliest a offset
match("ABC", "abc")        = (0, 0, 0)
match("", "test")         = (0, 0, 0)
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover contiguous matching, repeated-character ties, empty input, upper/lowercase differences, 512-character limits, and invalid text.

## Limitations

ASCII only; no normalization, tokenization, fuzzy matching or semantic similarity. The `O(nm)` dynamic program is educational and is not optimized for very long text or repeated large-scale corpus search.

## References

Gusfield, D. (1997). *Algorithms on strings, trees, and sequences: Computer science and computational biology*. Cambridge University Press. https://doi.org/10.1017/CBO9780511574931
