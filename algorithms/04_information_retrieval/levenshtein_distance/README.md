# Levenshtein Edit Distance (Printable ASCII)

## Why it belongs in the AI hub

Levenshtein distance measures how many single-character **insertions,
deletions, and substitutions** separate two strings. Search systems use
edit distance to suggest spelling corrections, expand queries and support
fuzzy term matching. A dynamic programming recurrence computes the
minimum number of edits (Wagner & Fischer, 1974).

The algorithm operates directly on **two strings**, not a search index
or a trained language model.

## Recurrence

For characters `a[0..n-1]` and `b[0..m-1]`, set

```text
D[0,j] = j
D[i,0] = i

D[i,j] = min(
    D[i-1,j] + 1,                             # delete
    D[i,j-1] + 1,                             # insert
    D[i-1,j-1] + (a[i-1] != b[j-1])           # substitute/match
)
```

Return `D[n,m]`. Transposing two adjacent characters counts as two
edits in general; the algorithm is **not** Damerau–Levenshtein distance.

Examples:

```text
distance("kitten", "sitting") = 3
distance("", "abc")         = 3
distance("cat", "cat")      = 0
distance("ab", "ba")        = 2
distance("A", "a")          = 1
```

## Shared contract

- Both inputs are strings containing **only printable ASCII code points
  U+0020 through U+007E**, inclusive, with lengths in **[0,512]**.
  Empty strings are allowed. Characters are case-sensitive.
- Reject non-ASCII, tabs, newline, NUL, other control characters,
  incorrect input types (dynamic languages), or strings over 512
  characters. This deliberate restriction avoids disagreements about
  Unicode normalization, bytes and grapheme clusters across languages.
- Output is the nonnegative **integer** number of unit-cost edits.
  Maximum output is 512 within the defined length limits.
- C reads NUL-terminated input buffers with enough storage and returns
  `bool`, storing into `size_t *distance_out` only after validation
  and successful computation; output remains unchanged on error.
  Internal NUL bytes are not representable in a C string.
  Rust accepts `&str` and returns `Result<usize,...>`.
  Python accepts `str` and returns `int`; R accepts two
  length-one character vectors and returns `integer`.
- The order of input strings does not affect the result.
  The algorithm uses two dynamic programming rows rather than
  allocating the full `(n+1)×(m+1)` grid.

## Complexity

Time: **O(n × m)**, where `n,m` are the input lengths.
Additional memory: **O(min(n,m))** using the shorter string for the
DP column dimension. The cost is exact, not approximate.

## Implementations

| Language | Folder |
| --- | --- |
| C | [levenshtein_distance_c](levenshtein_distance_c/) |
| Python | [levenshtein_distance_python](levenshtein_distance_python/) |
| Rust | [levenshtein_distance_rust](levenshtein_distance_rust/) |
| R | [levenshtein_distance_r](levenshtein_distance_r/) |

Run tests from the root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover textbook examples, symmetry, empty strings, substitution,
case-sensitive comparisons, boundaries, nonprintable characters,
Unicode rejection, and long input.

## Limitations

ASCII only, not suitable for normalized multilingual text without a
separate preprocessing layer. No transposition discount, learned edit
weights, phonetic matching, trie acceleration, spell-correction corpus
model, embedding similarity or GPU implementation. Exact edit distance
is a surface-form metric, **not a semantic similarity score**.

## References

Wagner, R. A., & Fischer, M. J. (1974). The string-to-string correction
problem. *Journal of the ACM, 21*(1), 168–173.
https://doi.org/10.1145/321796.321811
