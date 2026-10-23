# Sorted Postings List Intersection

## Why it belongs in the AI hub

A Boolean **AND** query in an inverted index finds documents containing
all requested terms. For two terms, it is enough to intersect their
sorted postings lists using a linear two-pointer merge rather than
scan the full document collection. This is a central retrieval operation
described by Manning et al. (2008).

This algorithm complements the repository's *Single-Term Inverted Postings
List* primitive, which produces sorted document IDs and frequencies.

## Definition

Given **strictly increasing** sequences of unique document IDs
`left` and `right`, advance the pointer of the smaller ID, emitting
a value and advancing both pointers when they are equal.

```text
left  = [0, 2, 4, 9]
right = [1, 2, 4, 8]
result = [2, 4]
```

Empty inputs produce an empty intersection. If both lists contain the
same document IDs, the result equals those IDs.

## Contract

- Input IDs must be **unsigned 31-bit integers** in `[0, 2,147,483,647]`,
  unique and strictly increasing within each input list.
- Reject duplicates, descending order, negative values, nonintegers and
  out-of-range IDs instead of silently sorting away data-quality errors.
- Empty lists are valid, but the nonempty list still must be validated.
- C accepts `uint32_t` buffers, counts and an output buffer with at least
  `min(n_left,n_right)` elements (may be NULL if the intersection must be
  empty due to an empty input); `out_count` must be non-null.
  Output and input buffers must **not overlap**. On invalid input, both
  output array and count remain unchanged.
- Python and R raise errors. Rust returns `Result<Vec<u32>, ...>`.
  R returns an integer vector, and all languages use **zero-based IDs**.
- C and Rust have typed unsigned buffers; C additionally rejects entries
  above 2,147,483,647 to keep the shared ID contract.

## Complexity

**O(n + m)** time and O(1) working memory beyond the returned list.
C writes into a caller-provided result; Python/Rust/R return up to
`O(min(n,m))` matching IDs.

## Implementations

| Language | Directory |
| --- | --- |
| C | [postings_intersection_c](postings_intersection_c/) |
| Python | [postings_intersection_python](postings_intersection_python/) |
| Rust | [postings_intersection_rust](postings_intersection_rust/) |
| R | [postings_intersection_r](postings_intersection_r/) |

## Examples and tests

```text
intersect([0,2,4,9],[1,2,4,8]) -> [2,4]
intersect([], [1,4])           -> []
intersect([3,3], [3])          -> error (duplicate input)
intersect([5,2], [2])          -> error (unsorted input)
```

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

This is an exact **two-list** merge. It does not tokenize queries, build
indexes, use skip pointers, merge posting frequencies, implement OR/NOT
queries, or optimize the order of intersecting many postings lists.
CUDA would need a separate GPU-oriented parallel merge design.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to
information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
