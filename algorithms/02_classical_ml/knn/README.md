# Exact K-Nearest Neighbors (KNN)

**Stage:** Classical Machine Learning · **Languages:** C, Python, Rust, R.

## Why it belongs here

Nearest-neighbor search is a simple nonparametric foundation for
classification, recommendation and vector retrieval. Cover and Hart (1967)
analyzed the nearest-neighbor decision rule. This block isolates the
**neighbor-search operation** that later classifiers and indexes can reuse.

## What it does

For a finite dataset of `n` points in `d` dimensions, a query point
`q` and a positive integer `k`, calculate the Euclidean distance from
every dataset point to `q`. Return indices of the **k smallest distances**.

Example:

```text
points: [[0,0], [5,5], [1,1], [0,2]]
query: [0,1], k=2
result: [0, 3]  (both distance 1; lower index breaks the tie)
```

## Shared contract

- `n ≥ 1`, `d ≥ 1`, integer `1 ≤ k ≤ n`.
- Data points and query are finite real-valued and have exactly `d`
  components. All arrays must be rectangular.
- Output is an array of **zero-based original sample indices**, ordered
  by increasing Euclidean distance; **ties by smaller original index**.
  R deliberately returns zero-based indices to match the other languages.
- Distances use a numerically stable recurrence (`hypot` or scaled sum of
  squares) to avoid unnecessary squared-norm overflow. If any distance
  is not representable, **fail the entire query**, rather than silently
  dropping a sample or returning infinity.
- C accepts row-major flat samples, sample count, dimension, query,
  `k`, and a caller-owned output array. Buffer lengths/non-aliasing are
  caller responsibilities; outputs are unchanged on failure.
- Python and R raise an error for invalid data; Rust returns `Result`.

**This implementation returns neighbors, not class labels.** Predicting
a class requires an additional label aggregation policy and is left to
a separate focused contribution.

## Complexity

- All versions: **O(nd + n log n)** time, using brute-force distances
  and a full sort; **O(n)** additional memory for distance/rank pairs
  and O(k) output storage.
- No search tree, ANN index, GPU acceleration or performance claim.

## Implementations

| Language | Directory |
| --- | --- |
| C | [knn_c](knn_c/) |
| Python | [knn_python](knn_python/) |
| Rust | [knn_rust](knn_rust/) |
| R | [knn_r](knn_r/) |

## Tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Cases cover ties, sorted zero-based output, `k=n`, non-rectangular data,
empty inputs, invalid `k`, NaN/infinity, and unrepresentably large distances.

## Limitations

Brute-force search becomes costly for large datasets; future work may add
a KD-tree, HNSW index, vectorized batches or optional CUDA acceleration.
This block does not provide label prediction, cosine metrics, missing-data
imputation, dynamic updates or approximate-recall guarantees.

## References

Cover, T. M., & Hart, P. E. (1967). Nearest neighbor pattern classification.
*IEEE Transactions on Information Theory, 13*(1), 21–27.
https://doi.org/10.1109/TIT.1967.1053964
