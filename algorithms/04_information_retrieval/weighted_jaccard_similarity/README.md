# Weighted Jaccard Similarity

## Why it belongs in this repository

Weighted Jaccard is a nonnegative-vector generalization of Jaccard set
overlap. It compares corresponding token or feature weights using minimum
and maximum contributions, which can be more informative than binary token
presence when comparing search documents and sparse weighted features.
For zero-or-one weights, it reduces to ordinary Jaccard similarity
(Manning et al., 2008).

## Mathematical definition

For two aligned, nonnegative feature-weight vectors x and y:

    weighted_J(x, y) = sum_i min(x_i, y_i) / sum_i max(x_i, y_i)

An all-zero pair is assigned similarity 1 by documented convention.
A zero vector compared with a nonzero vector has similarity 0.
The score lies in [0,1]. Inputs are scaled by their largest coordinate
before accumulation to avoid overflow from large finite weights.

## Contract

- Inputs: equally sized, nonempty, ordered vectors of finite,
  **nonnegative** real weights. Index i must refer to the same feature
  in both vectors. Duplicate tokens must be aggregated before use.
- Output: a scalar similarity in [0,1], with 1 for identical vectors
  and 0 for vectors whose positive entries have no overlap.
- Python raises ValueError for empty/mismatched/negative/nonfinite/
  nonnumeric weights, including Boolean values. R rejects complex/matrix
  or nonfinite/negative inputs. C returns false without changing output
  on invalid input, and Rust returns Err. C pointers must address n
  elements and must not be null.
- All-zero vectors return 1; no implicit normalization is performed.

## Complexity

O(n) time and O(1) auxiliary storage for n weighted features.

## Implementations

| Language | Folder |
| --- | --- |
| C | [weighted_jaccard_similarity_c](weighted_jaccard_similarity_c/) |
| Python | [weighted_jaccard_similarity_python](weighted_jaccard_similarity_python/) |
| Rust | [weighted_jaccard_similarity_rust](weighted_jaccard_similarity_rust/) |
| R | [weighted_jaccard_similarity_r](weighted_jaccard_similarity_r/) |

## Examples and tests

    J([1,2,0], [1,1,3]) = 1/3
    J([0,0], [0,0])     = 1
    J([1,0], [0,1])     = 0
    J([1,2], [1,2])     = 1

From the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is for *aligned, nonnegative feature weights*, not arbitrary signed
embeddings. It does not tokenize documents, build a term dictionary or
support binary hashing or sparse index representations. Extremely tiny
weights relative to the largest weight may underflow during scaling.
The all-zero convention may differ from external libraries.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008).
*Introduction to information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
