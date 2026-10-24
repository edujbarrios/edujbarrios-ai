# Exact Euclidean Top-K Vector Search

## AI motivation

Euclidean distance is a common similarity measure for nearest-neighbor
retrieval in embedding and feature spaces. An exact **top-K vector scan**
is a useful ground-truth baseline for comparing approximate methods and
alternative similarity measures. This primitive extends the repository's
pairwise Euclidean Distance and complements Exact Cosine Top-K
(Manning et al., 2008).

## Mathematical definition

For query `q ∈ R^d` and candidate rows `x_i ∈ R^d`:

```text
distance(q,x_i) = sqrt(sum_j (q[j] - x_i[j])^2)
result = indices of k smallest distances
```

Sort by ascending distance, then **ascending original zero-based
index** for exact ties. Duplicate candidates remain distinct rows.

To avoid avoidable overflow from squaring large finite differences,
calculate the norm using a running **hypotenuse** recurrence
`distance = hypot(distance, q[j] - x_i[j])`. A difference itself,
or its final norm, can still overflow double precision and is rejected.

## Worked example

```text
query      = [0,0]
candidates = [[1,0],[0,1],[3,4],[0,0],[-1,0]]
distances  = [1,1,5,0,1]
top_5      = [3,0,1,4,2]
```

## Shared contract

- Accept a nonempty finite-real query vector with `d>=1`, a
  **nonempty** `n×d` matrix of finite-real candidates and integer
  cutoff `1<=k<=n`.
- Validate **every** candidate row, even if it ranks outside top-K.
  Zero vectors, duplicate candidates and zero-distance matches are
  permitted. Reject NaN/Inf, mismatched dimensions, invalid cutoff,
  overflowed coordinate differences or nonrepresentable distances.
- Return `k` original **zero-based row indices** ordered by
  (distance,index). The list is not a probability distribution.
- C accepts flat row-major candidates and caller-owned `size_t
  indices[k]`; the result remains unchanged on any error. Rust uses
  a row-major flat array and returns `Result<Vec<usize>,...>`.
  Python takes nested arrays and returns a list; R accepts a base
  numeric matrix and returns a zero-based integer vector.
- C requires sufficiently allocated nonoverlapping input/output
  buffers. Python rejects booleans-as-numbers; R rejects nonfinite
  data and noninteger cutoff values.
- This implementation performs a full scan and sort. It contains no
  prebuilt index and offers no sublinear retrieval guarantee.

## Complexity

Compute n distances in **O(n*d)** time and sort n distance-index pairs
in **O(n log n)** time. Uses O(n) scratch memory in C/Rust/R;
Python also copies the input candidate matrix (O(n*d) memory).

## Implementations

| Language | Directory |
| --- | --- |
| C | [euclidean_top_k_c](euclidean_top_k_c/) |
| Python | [euclidean_top_k_python](euclidean_top_k_python/) |
| Rust | [euclidean_top_k_rust](euclidean_top_k_rust/) |
| R | [euclidean_top_k_r](euclidean_top_k_r/) |

## Tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover ties, duplicates, zero vectors, ranking order, stable
hypot for large distances, invalid data outside K, malformed
dimensions and genuine intermediate overflow.

## Limitations

Brute-force exact search only. No batched queries, approximate index,
hierarchical graph, quantization, metric learning, streaming updates,
or CUDA kernel. The meaning of Euclidean distance depends on feature
scaling; standardize or normalize data as appropriate **using
training-only statistics** to prevent leakage.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008).
*Introduction to information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
