# Longest Palindromic Substring (ASCII)

## Information retrieval context

Detecting repeated symmetry in contiguous character spans is useful for text-pattern exercises, sequence analysis, and string indexing demonstrations. This algorithm returns the longest **palindromic substring**, not a palindromic subsequence, using center expansion (Gusfield, 1997).

## Definition and algorithm

A substring `s[i..j]` is a palindrome when reading it forwards and backwards gives the same sequence. For every character position, expand both an odd-length center `(i,i)` and an even-length center `(i,i+1)` while the endpoints match. Track the **maximum length**; break ties by selecting the **lowest zero-based start offset**.

Examples:

```text
longest_palindromic_substring("babad") = (3, 0)  # "bab", not later "aba"
longest_palindromic_substring("cbbd")  = (2, 1)  # "bb"
longest_palindromic_substring("aaaa")  = (4, 0)
longest_palindromic_substring("abc")   = (1, 0)
longest_palindromic_substring("")      = (0, 0)
```

## Input/output contract

- A case-sensitive **printable ASCII** string (U+0020–U+007E) of length **0..512**; ASCII spaces and punctuation count as characters. Reject non-ASCII, control characters and excessive length. Python and R also reject malformed types.
- Return a **(length, zero-based start offset)** pair, with `(0,0)` for empty input. Any nonempty valid string has a palindrome of length at least 1.
- C accepts NUL-terminated text plus two **distinct non-null** `size_t *` outputs, returns `bool`, and leaves both outputs unchanged on validation failure. Embedded NUL is not representable in C strings.
- Python returns `tuple[int,int]`, Rust `Result<(usize,usize),&str>`, R `list(length=integer,start=integer)`.
- The input is never modified. There is no Unicode normalization, ignoring case, or skipping punctuation.

## Complexity

At most 2 centers per character, with up to O(n) comparisons per expansion: **O(n²)** time and **O(1)** extra workspace (excluding the returned result).

## Implementations

| Language | Folder |
| --- | --- |
| C | [longest_palindromic_substring_c](longest_palindromic_substring_c/) |
| Python | [longest_palindromic_substring_python](longest_palindromic_substring_python/) |
| Rust | [longest_palindromic_substring_rust](longest_palindromic_substring_rust/) |
| R | [longest_palindromic_substring_r](longest_palindromic_substring_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the root. Tests cover odd/even palindromes, stable ties, punctuation, maximum-length input, empty input, invalid types and non-ASCII data.

## Limitations

The O(n²) center-expansion method is intended for small inputs; Manacher's algorithm can achieve O(n) time for longer texts. Exact byte/character comparisons do not capture linguistic equivalence.

## References

Gusfield, D. (1997). *Algorithms on strings, trees, and sequences: Computer science and computational biology*. Cambridge University Press. https://doi.org/10.1017/CBO9780511574931
