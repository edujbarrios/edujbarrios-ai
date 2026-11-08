# Rabin–Karp Substring Search

## Why it belongs in this repository

Exact substring matching underlies document highlighting, duplicate detection, and lexical information retrieval. Rabin–Karp uses a rolling fingerprint to compare candidate text windows efficiently (Karp & Rabin, 1987).

## Algorithm

Use the polynomial hash `h(s) = Σ code(s[i]) × 257^(m-1-i) mod 1000000007` for a pattern of length `m`. Update the length-`m` text window in constant time by removing the outgoing character and adding the incoming character. **Verify candidate substrings by direct equality** whenever hashes match, so hash collisions never cause false matches.

## Contract

- `text` and `pattern`: printable ASCII strings (U+0020–U+007E), each 0–512 characters; exact, case-sensitive matching. Reject invalid types (dynamic languages), non-ASCII/control characters, and inputs longer than 512.
- Return **all zero-based start offsets in ascending order**, including overlapping matches; return an empty collection for no matches.
- An empty pattern matches every boundary: `search("abc", "") = [0,1,2,3]`; `search("", "") = [0]`.
- Python returns `list[int]`, Rust returns `Result<Vec<usize>, &str>`, R returns an integer vector.
- C accepts NUL-terminated input strings, caller-owned `size_t positions[513]` and `size_t *count_out`; `positions` must have capacity 513 and outputs are unchanged on failure. An embedded NUL cannot be represented in a C string.
- The rolling hash is deterministic, not cryptographic; equality verification makes the **result exact**, irrespective of collisions.

## Complexity

Precomputation takes **O(m)** time and **O(1)** extra working space. Rolling over the text is **O(n)** plus **O(m)** for each hash-equal candidate verification (worst-case **O(nm)**), and **O(k)** result storage for `k` matches (fixed 513-slot caller output in C). No randomness is used.

## Implementations

| Language | Folder |
| --- | --- |
| C | [rabin_karp_substring_search_c](rabin_karp_substring_search_c/) |
| Python | [rabin_karp_substring_search_python](rabin_karp_substring_search_python/) |
| Rust | [rabin_karp_substring_search_rust](rabin_karp_substring_search_rust/) |
| R | [rabin_karp_substring_search_r](rabin_karp_substring_search_r/) |

## Examples and tests

```text
search("aaaa", "aa") = [0, 1, 2]
search("abababa", "aba") = [0, 2, 4]
search("abc", "d") = []
search("", "") = [0]
```

From the repository root, run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`.

Tests include overlaps, hash updates, empty strings, case sensitivity, maximum lengths, and invalid inputs.

## Limitations

ASCII-only, no normalization or fuzzy matching. A fixed modulus can cause many hash collisions on adversarial input; all such candidates are verified. This is an educational implementation, not a secure fingerprint or a full-text index.

## References

Karp, R. M., & Rabin, M. O. (1987). Efficient randomized pattern-matching algorithms. *IBM Journal of Research and Development, 31*(2), 249–260. https://doi.org/10.1147/rd.312.0249
