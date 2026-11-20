# Sorted Lexicon Prefix Search

## Why it belongs in AI

Dictionary lookup underpins search interfaces, query completion, token
dictionaries and lexicon-based information retrieval. With sorted terms,
binary search finds the first possible prefix match; all matching words
appear together, so a forward scan enumerates completions (Manning et al., 2008).

## Algorithm

1. Verify that the dictionary contains distinct, strictly increasing terms.
2. Use binary search to find the first term lexicographically >= the prefix.
3. Return the zero-based positions of successive words that start with it.

The dictionary is **not** sorted by the function: preserving original
indices matters for postings or metadata kept in parallel arrays.
The implementation deliberately uses a sorted array, not a trie.
It supports exact prefix filtering, not typo tolerance or ranking.

## Contract

- Inputs: sorted, unique lowercase ASCII words (a-z only), each
  containing 1 to 512 characters; an ASCII lowercase prefix of 0 to
  512 characters. An empty dictionary is permitted.
- Output: a list of zero-based **dictionary indices**, in ascending order.
  The empty prefix selects every word; a missing prefix returns none.
- Invalid inputs: unsorted/duplicate words, empty words, mixed-case,
  non-ASCII, punctuation, overlong words or prefixes, or missing values.
  C requires nonnull pointers, even when n=0, and caller-owned output
  capacity for n indices; failure leaves output/count unchanged.
- Comparison is exact bytewise lexicographic order, consistent across
  all four implementations. No randomization.

For dictionary [ant, antenna, ape, bat, batch, cat], prefix an returns
[0, 1]; prefix ba returns [3, 4]; empty prefix returns [0, 1, 2, 3, 4, 5].

## Complexity

For n words of maximum length L and k matches, validation is O(nL).
Binary search then uses O(L log n) comparisons followed by O(kL)
prefix comparisons. Output takes O(k) memory; the algorithm uses O(1)
additional search memory apart from output (Python also builds a copy
of the input for validation). Preparing an unsorted lexicon is outside
this contract.

## Implementations

| Language | Directory |
| --- | --- |
| C | [sorted_lexicon_prefix_search_c](sorted_lexicon_prefix_search_c/) |
| Python | [sorted_lexicon_prefix_search_python](sorted_lexicon_prefix_search_python/) |
| Rust | [sorted_lexicon_prefix_search_rust](sorted_lexicon_prefix_search_rust/) |
| R | [sorted_lexicon_prefix_search_r](sorted_lexicon_prefix_search_r/) |

Run the repository checks:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is an in-memory exact dictionary search, not fuzzy autocomplete.
It does not support Unicode collation, language-dependent casing,
term weighting, edits or dynamic insertion. Input validation costs
O(nL) on each call; a production immutable index should validate
once at build time and reuse that invariant for repeated queries.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to information
retrieval*. Cambridge University Press. https://nlp.stanford.edu/IR-book/
