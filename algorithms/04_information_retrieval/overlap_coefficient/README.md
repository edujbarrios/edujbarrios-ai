# Overlap Coefficient

## Why it belongs in this repository

The overlap coefficient (also called Szymkiewicz–Simpson similarity)
compares the intersection of two sets relative to the *smaller* set.
Unlike Jaccard or Dice similarity, it returns 1 when one nonempty set
is fully contained in the other. This makes it useful for comparing
a short query's token set against a longer document (Manning et al., 2008).

## Definition

For nonempty sets A and B:

    overlap(A, B) = |A intersection B| / min(|A|, |B|)

Inputs are sequences of signed 32-bit token IDs, deduplicated before
counting. Both empty sets return 1 by explicit convention; exactly one
empty set yields 0. Scores range from 0 to 1.

## Contract

- Inputs: ordered or unordered sequences of signed 32-bit integer IDs;
  duplicates are allowed but ignored.
- Python accepts lists/tuples and rejects Boolean, non-integer and
  out-of-range values with ValueError. R accepts finite whole-number
  vectors in the signed 32-bit range. C uses int32_t; Rust uses i32.
- Output: a finite real similarity in [0,1].
- C leaves the supplied result untouched on error and accepts null
  input pointers *only* for empty sequences. All nonnull buffers must
  cover the specified lengths.

## Complexity

O(n log n + m log m) time to sort and deduplicate the inputs, plus
O(n + m) auxiliary memory.

## Implementations

| Language | Folder |
| --- | --- |
| C | [overlap_coefficient_c](overlap_coefficient_c/) |
| Python | [overlap_coefficient_python](overlap_coefficient_python/) |
| Rust | [overlap_coefficient_rust](overlap_coefficient_rust/) |
| R | [overlap_coefficient_r](overlap_coefficient_r/) |

## Examples and tests

    overlap([1, 1, 2], [1, 2, 3, 4]) = 1
    overlap([1, 2], [2, 3])         = 0.5
    overlap([], [])                 = 1
    overlap([], [7])                = 0

From the repository root run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

The algorithm treats IDs as unweighted sets, not multisets.
Tokenization, vocabulary alignment and fuzzy text matching are out of
scope. The chosen empty-set convention must be consistent downstream.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008).
*Introduction to information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
