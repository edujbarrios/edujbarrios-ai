# Knuth–Morris–Pratt (KMP) Substring Search

## Why it belongs in this repository

Exact literal substring matching is a building block of lexical information retrieval, query highlighting, and rule-based text processing. Knuth, Morris, and Pratt (1977) showed how a prefix (failure) table avoids reconsidering characters after a mismatch.

## What it does

Build the length-`m` prefix table `pi`, where `pi[i]` is the length of the longest proper prefix of `pattern[:i+1]` that is also a suffix. On each text mismatch, fall back from matched prefix length `j` to `pi[j-1]` rather than restarting. Report **every** occurrence, including overlapping matches.

## Contract

- `text` and `pattern` must be printable ASCII (U+0020 through U+007E), **0–512 characters** each; matching is exact and case-sensitive.
- Return all **zero-based start offsets** in ascending order. No match returns an empty collection.
- An empty pattern matches **every boundary**, including the end: `search("abc", "") = [0, 1, 2, 3]`; `search("", "") = [0]`.
- Reject control characters, non-ASCII, oversize strings, and invalid types in dynamic languages. C strings are NUL-terminated and therefore cannot represent embedded NUL.
- Python returns `list[int]`; Rust returns `Result<Vec<usize>, &str>`; R returns an integer vector.
- C returns `bool` and writes into caller-provided `size_t positions[513]` and `size_t *count_out`. The caller must provide room for **513 positions**; both outputs remain unchanged on validation failure. Invalid/null output pointers are rejected.

## Complexity

Building the prefix table costs **O(m)**; scanning costs **O(n)**. Total time is **O(n + m)**, with O(m) temporary space and O(k) result space for k matches (a fixed 513-slot caller buffer in C). The approach is exact, not approximate.

## Implementations

| Language | Folder |
| --- | --- |
| C | [kmp_substring_search_c](kmp_substring_search_c/) |
| Python | [kmp_substring_search_python](kmp_substring_search_python/) |
| Rust | [kmp_substring_search_rust](kmp_substring_search_rust/) |
| R | [kmp_substring_search_r](kmp_substring_search_r/) |

## Examples and tests

```text
search("aaaa", "aa")                 = [0, 1, 2]
search("ABABDABACDABABCABAB", "ABABCABAB") = [10]
search("abc", "z")                   = []
search("", "")                      = [0]
```

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The tests exercise fallback, overlaps, absent patterns, case sensitivity, empty strings, 512-character boundaries, and invalid inputs.

## Limitations

Only literal ASCII matching: no Unicode normalization, wildcards, fuzzy matching, tokenization, or ranking. For large texts or repeated queries, a production search index may be more appropriate.

## References

Knuth, D. E., Morris, J. H., Jr., & Pratt, V. R. (1977). Fast pattern matching in strings. *SIAM Journal on Computing, 6*(2), 323–350. https://doi.org/10.1137/0206024
