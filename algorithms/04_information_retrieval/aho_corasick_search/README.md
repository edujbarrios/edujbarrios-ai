# Aho–Corasick Multi-Pattern Search

## Why it belongs in AI

Search engines, corpus filters and natural-language preprocessing frequently
need to match **many patterns in one text**. Aho–Corasick combines a trie
with failure links to identify all matches in a single traversal of the
text. It was developed for efficient bibliographic search
(Aho & Corasick, 1975).

## Algorithm

1. Insert every search pattern into a trie of Unicode code points,
   remembering its original zero-based **pattern index**.
2. Breadth-first, compute each state's failure link to its longest
   proper suffix that is also a trie prefix.
3. Link each state to the closest failure ancestor that ends one or
   more patterns (an **output link**).
4. Scan text left to right. On a missing transition, follow failure
   links until a transition exists or the root is reached.
5. Emit each terminal match at the current state and through its
   output links. Sort results by increasing start offset and
   then original pattern index.

An output link avoids repeatedly copying lists of inherited matches
into trie nodes. Equal or duplicate pattern strings retain distinct
pattern indices. Overlapping matches are returned.

## Contract

- `aho_corasick_search(text, patterns)` takes one string with at
  most **100000 Python Unicode code points**, including the empty string,
  and a list/tuple of **1..256 nonempty strings**, whose total length
  is at most **8192 code points**.
- Comparisons are exact and case-sensitive. There is no Unicode
  normalization; offsets are **zero-based code-point offsets**,
  not UTF-8 byte offsets.
- Output is a list of `(pattern_index, start_offset)` tuples
  sorted first by start offset and then by pattern index.
- To bound memory, more than **200000 matches** raises `ValueError`
  rather than truncating results. Invalid inputs and limits also
  raise `ValueError`.
- Inputs are not modified. No third-party dependencies or randomization.

## Worked example

```python
aho_corasick_search("ushers", ["he", "she", "his", "hers"])
# [(1, 1), (0, 2), (3, 2)]
```

At offset 2, both `he` and `hers` match, so the smaller
original pattern index is returned first.

## Complexity

Let `P` be the total pattern length, `L` the maximum pattern
length, `T` the text length, and `Z` the number of returned matches.
With dictionary transitions, building failure links costs at most
**O(P L)** time in this straightforward implementation, scanning
takes **O(T + Z)** time, and sorting the output costs
**O(Z log Z)** time. Trie and match storage are **O(P + Z)**.
The match cap bounds output memory on highly repetitive inputs.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [aho_corasick_search_python](aho_corasick_search_python/) |

Other language variants are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

The tests compare against a naive exhaustive matcher, covering Unicode
code-point offsets, duplicate patterns, suffix matches, overlaps,
empty text, invalid types and length limits.

## Limitations

The implementation builds a new trie on every call and materializes
all results before sorting, so it is not a streaming search interface.
There is no case folding, locale-aware collation, tokenization,
normalization, wildcard matching or fuzzy matching. Very repetitive
texts can exceed the intentional output cap.

## References

Aho, A. V., & Corasick, M. J. (1975). Efficient string matching:
An aid to bibliographic search. *Communications of the ACM, 18*(6),
333–340. https://doi.org/10.1145/360825.360855
