# Bray–Curtis Dissimilarity

## Why it belongs in this repository

Bray–Curtis dissimilarity compares nonnegative feature vectors relative
to their total magnitude. Unlike Euclidean distance it emphasizes
relative differences; it is used for comparing count or abundance
profiles and illustrates a bounded, scale-aware dissimilarity
(Bray & Curtis, 1957).

## Mathematical definition

For nonnegative vectors x and y:

    d(x, y) = sum_i |x_i - y_i| / sum_i (x_i + y_i)

Both zero vectors return 0 by documented convention; zero versus
nonzero returns 1. For all other nonnegative inputs the value lies
in [0,1]. A common scale is applied before subtraction and summation,
reducing overflow when entries are very large.

## Contract

- Inputs: nonempty vectors of equal length with finite **nonnegative**
  real coordinates, without implicit normalization.
- Output: finite scalar in [0,1].
- Invalid input: empty/mismatched vectors, negative, nonfinite, nonreal
  values (and booleans in Python). Python raises ValueError, R errors,
  Rust returns Err and C returns false without changing the output.
- C input buffers must each hold n doubles; pointers including output
  must be nonnull. Output memory may not alias input memory.
- This is a dissimilarity, **not** a mathematical metric in general.

## Complexity

O(n) time and O(1) auxiliary storage for n feature coordinates.

## Implementations

| Language | Folder |
| --- | --- |
| C | [bray_curtis_dissimilarity_c](bray_curtis_dissimilarity_c/) |
| Python | [bray_curtis_dissimilarity_python](bray_curtis_dissimilarity_python/) |
| Rust | [bray_curtis_dissimilarity_rust](bray_curtis_dissimilarity_rust/) |
| R | [bray_curtis_dissimilarity_r](bray_curtis_dissimilarity_r/) |

## Examples and tests

    d([1,2],[2,4]) = 3/9 = 1/3
    d([0,0],[0,0]) = 0
    d([0,1],[1,0]) = 1

From the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

The implementation does not work on negative-valued embeddings,
calculate feature weights, or provide sparse-matrix acceleration.
If inputs vary by extreme orders of magnitude, tiny entries may
underflow after scaling. The both-zero convention is explicitly chosen.

## References

Bray, J. R., & Curtis, J. T. (1957). An ordination of the upland forest
communities of southern Wisconsin. *Ecological Monographs, 27*(4),
325–349. https://doi.org/10.2307/1942268
