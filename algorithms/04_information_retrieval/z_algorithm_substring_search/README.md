# Z-Algorithm Substring Search (ASCII)

## Information retrieval context

The Z-algorithm computes the length of the common prefix between a sequence and every one of its suffixes. Applied to `pattern + sentinel + text`, it finds exact substring occurrences in **O(n+m)** time, including overlaps (Gusfield, 1997).

## Algorithm

Let `P` be the pattern of length `m` and `T` the text of length `n`. Construct the integer sequence `A = P + [0] + T`, using **0 as a sentinel**. Printable ASCII input bytes range from 32 through 126, so the sentinel cannot occur in either string.

Define `Z[i]` as the number of leading elements of `A` that match the prefix of the suffix beginning at `i`. Maintain an interval `[left,right)` whose prefix comparison is known. For each new offset `i`, reuse the corresponding prefix Z-value inside the interval before extending with direct comparisons. Every extension advances the right bound, giving linear total work.

For `i >= m+1` in the concatenated array, record zero-based text offset `i-m-1` when `Z[i] >= m`. When `m=0`, return every boundary offset from `0` through `n`, without building the Z-array.

## Contract

- `text` and `pattern` are **printable ASCII** strings (U+0020–U+007E), each length **0..512**, case-sensitive, without normalization. Reject non-ASCII, control characters and overly long strings.
- Return an ascending list/vector of **all zero-based exact match positions**, including overlaps. `search("abc","") = [0,1,2,3]`, and `search("","") = [0]`.
- C accepts NUL-terminated strings plus a caller-owned `size_t positions[513]` and a distinct `size_t *count_out`. The array requires **513 entries**. On invalid input, both outputs remain unchanged. Embedded NULs cannot be encoded in a C string.
- Python returns `list[int]` and rejects wrong types; Rust returns `Result<Vec<usize>,&str>`; R returns an integer vector and rejects invalid types.
- No input mutation, approximate string matching or Unicode handling.

## Complexity

**O(n+m)** time and **O(n+m)** auxiliary storage for the concatenation and Z-array, plus O(k) result storage for k matches. The fixed maximum concatenated length is 1025 integer values.

## Implementations

| Language | Folder |
| --- | --- |
| C | [z_algorithm_substring_search_c](z_algorithm_substring_search_c/) |
| Python | [z_algorithm_substring_search_python](z_algorithm_substring_search_python/) |
| Rust | [z_algorithm_substring_search_rust](z_algorithm_substring_search_rust/) |
| R | [z_algorithm_substring_search_r](z_algorithm_substring_search_r/) |

## Examples and tests

```text
search("banana","ana")  = [1,3]
search("aaaa","aa")     = [0,1,2]
search("abcabc","abc")  = [0,3]
search("abc","xyz")     = []
search("","")           = [0]
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the root. Tests cover overlaps, the sentinel boundary, case-sensitive matches, punctuation, empty inputs, maximum-length inputs and invalid ASCII.

## Limitations

The Z-algorithm handles one exact pattern at a time. It is not a full-text index, fuzzy matcher, or Unicode grapheme matcher. For large collections of patterns, different indexing structures may be more suitable.

## References

Gusfield, D. (1997). *Algorithms on strings, trees, and sequences: Computer science and computational biology*. Cambridge University Press. https://doi.org/10.1017/CBO9780511574931
