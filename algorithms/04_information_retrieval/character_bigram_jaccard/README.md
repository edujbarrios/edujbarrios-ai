# Character Bigram Jaccard Similarity

## Information retrieval context

Character n-grams are useful for lexical deduplication, approximate entity matching and retrieval where similar spellings should share features. This algorithm compares the **sets** of consecutive pairs (bigrams), not their occurrence counts (Manning et al., 2008).

## Mathematical definition

For string `s` define `B(s) = {s[i:i+2] | 0 <= i < len(s)-1}`. Then:

```text
J(a,b) = |B(a) ∩ B(b)| / |B(a) ∪ B(b)|
```

When the union is empty, define the result as **1.0**, even if one or both strings are distinct one-character strings; this is explicitly a comparison of *bigram sets*, not character equality. Duplicated bigrams count only once.

## Contract

- Input: two case-sensitive **printable ASCII strings** (U+0020..U+007E) of lengths **0..512**, including empty strings. Reject invalid types (Python/R), control characters and non-ASCII data.
- Output: one finite numeric Jaccard score in **[0,1]**. Both sets empty yields `1.0`; one empty yields `0.0`; otherwise use set intersection/union.
- C takes NUL-terminated strings and a distinct caller-owned `double *similarity_out` that stays unchanged on failure. A C string cannot represent a literal embedded NUL. Rust returns `Result<f64,&str>`, Python returns `float`, R returns one numeric scalar.
- This is **binary presence** Jaccard, not weighted or multiset Jaccard. It does not lowercase or normalize punctuation.

## Complexity

For fixed printable ASCII alphabet size 95, the two presence tables each use **95² = 9,025** flags. Building bigram sets requires **O(|a|+|b|)** time, followed by an **O(95²)** overlap scan; working memory is **O(95²)**. No hash collisions or external dependencies.

## Implementations

| Language | Folder |
| --- | --- |
| C | [character_bigram_jaccard_c](character_bigram_jaccard_c/) |
| Python | [character_bigram_jaccard_python](character_bigram_jaccard_python/) |
| Rust | [character_bigram_jaccard_rust](character_bigram_jaccard_rust/) |
| R | [character_bigram_jaccard_r](character_bigram_jaccard_r/) |

## Examples and tests

```text
score("banana", "bandana") = 3/5 = 0.6
score("aaaa", "aa")       = 1.0
score("abcd", "cdef")     = 1/5 = 0.2
score("abc", "xyz")       = 0.0
score("a", "b")           = 1.0   # both bigram sets empty
score("a", "ab")          = 0.0
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict`. Tests cover exact set semantics, repeated bigrams, empty/single-character cases, symmetry, case sensitivity, ASCII boundaries, 512-character input lengths, and malformed strings.

## Limitations

For short strings, particularly lengths below two, the empty-set convention may be unintuitive. This method does not capture word semantics, and its bigram sets ignore term frequency and positions.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to information retrieval*. Cambridge University Press. https://doi.org/10.1017/CBO9780511809071
