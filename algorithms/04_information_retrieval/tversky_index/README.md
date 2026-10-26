# Tversky Index

## Why it belongs in this repository

The Tversky index generalizes Jaccard and Dice similarity with separate
penalties for elements unique to each set. It supports asymmetric
query-to-document comparisons and recall-versus-precision tradeoffs
in information retrieval (Tversky, 1977).

## Mathematical definition

For sets A and B and strictly positive finite alpha, beta:

    T(A,B) = |A ∩ B| /
             (|A ∩ B| + alpha |A \ B| + beta |B \ A|)

Alpha=beta=1 yields Jaccard; alpha=beta=0.5 yields Dice.
Swapping A and B can change the score when alpha != beta.
Both empty sets score 1; exactly one empty set scores 0.

## Contract

- Inputs: sequences of signed 32-bit integer IDs (duplicates ignored).
  Alpha and beta must each be finite and strictly positive.
- Output: finite scalar in [0,1], with 1 for equal nonempty sets.
- Python accepts lists/tuples, rejects Boolean/noninteger/out-of-range
  token IDs and invalid parameters with ValueError. R accepts
  one-dimensional integer-valued numeric vectors within the int32 range.
  Rust uses i32 slices; C uses int32_t buffers.
- C returns false on invalid input/allocation failure or nonfinite
  denominator, with its output unchanged. Null input pointers are
  permitted only for empty sequences. Output pointer must be writable.
- Extreme parameters that overflow the finite denominator are rejected.

## Complexity

O(n log n + m log m) time and O(n+m) auxiliary space for sorting and
deduplicating two ID sequences.

## Implementations

| Language | Folder |
| --- | --- |
| C | [tversky_index_c](tversky_index_c/) |
| Python | [tversky_index_python](tversky_index_python/) |
| Rust | [tversky_index_rust](tversky_index_rust/) |
| R | [tversky_index_r](tversky_index_r/) |

## Examples

    T([1,2], [2,3,4], alpha=1, beta=1)    = 1/4
    T([1,2], [2,3,4], alpha=0.5, beta=2)  = 2/11
    T([], [], alpha=1, beta=1)           = 1

Run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Tokens are treated as unweighted *sets*, not bags or ordered sequences.
No tokenization, indexing or weighted-feature scoring is performed.
The empty-set convention is a deliberate implementation choice.

## Reference

Tversky, A. (1977). Features of similarity. *Psychological Review, 84*(4),
327–352. https://doi.org/10.1037/0033-295X.84.4.327
