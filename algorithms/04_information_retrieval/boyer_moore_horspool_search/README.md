# Boyer–Moore–Horspool Substring Search

## Information retrieval context

Boyer–Moore–Horspool is an exact substring search technique that compares a pattern against text windows **right-to-left** and skips windows using a precomputed **bad-character shift** table. It is particularly practical when many text positions can be skipped without examining every character (Horspool, 1980).

## Algorithm

For a pattern of length `m > 0`, initialize `shift[ch] = m` for every printable ASCII character. For pattern indices `j = 0..m-2`, set:

```text
shift[pattern[j]] = m - 1 - j
```

For each candidate text window starting at `i`, compare from `pattern[m-1]` backwards. If every character matches, record `i`. After **every** window (even a match), move forward by `shift[text[i+m-1]]`. All shifts are at least one, allowing **overlapping** occurrences without an extra pass.

## Contract

- Inputs `text` and `pattern` are **printable ASCII** (U+0020–U+007E), each length **0..512**. Matching is case-sensitive, exact, and includes spaces and punctuation.
- Return **all zero-based start offsets**, sorted in ascending order, including overlaps; return an empty vector if none. If `pattern` is empty, match **every boundary**: `search("abc", "") = [0,1,2,3]`. If both are empty, return `[0]`.
- Python rejects invalid types, non-ASCII/control characters and strings longer than 512, returning `list[int]`. Rust returns `Result<Vec<usize>,&str>`; R returns an integer vector.
- C accepts NUL-terminated strings and a distinct caller-owned `size_t positions[513]` output with `size_t *count_out`. The array must have capacity **513**; outputs are unchanged on validation failure. Embedded NUL cannot be represented inside C strings.
- The skip table covers the 128 ASCII code positions, but only printable ASCII is accepted.

## Complexity

Preprocessing is **O(m + 128)** time and O(128) memory. Search is **O(nm)** worst-case, but can skip many windows in favorable inputs. Result storage is O(k) for `k` occurrences; the C caller provides capacity 513.

## Implementations

| Language | Folder |
| --- | --- |
| C | [boyer_moore_horspool_search_c](boyer_moore_horspool_search_c/) |
| Python | [boyer_moore_horspool_search_python](boyer_moore_horspool_search_python/) |
| Rust | [boyer_moore_horspool_search_rust](boyer_moore_horspool_search_rust/) |
| R | [boyer_moore_horspool_search_r](boyer_moore_horspool_search_r/) |

## Examples and tests

```text
search("ABAAABCD", "ABC") = [4]
search("aaaa", "aa")      = [0,1,2]
search("abababa", "aba")  = [0,2,4]
search("abc", "XYZ")      = []
search("", "")           = [0]
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover overlaps, long skip shifts, empty text/pattern, matches at the text ends, maximum lengths, case sensitivity and invalid types or characters.

## Limitations

This is a straightforward single-pattern Horspool search, not the full Boyer–Moore algorithm with good-suffix shifts. Adversarial repeated patterns can cause quadratic behavior. The implementation is ASCII-only, with no Unicode normalization or approximate matching.

## References

Horspool, R. N. (1980). Practical fast searching in strings. *Software: Practice and Experience, 10*(6), 501–506. https://doi.org/10.1002/spe.4380100608
