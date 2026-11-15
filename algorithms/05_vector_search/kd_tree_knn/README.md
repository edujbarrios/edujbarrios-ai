# Exact K-D Tree K-Nearest Neighbors

## Why it belongs in this repository

Nearest-neighbor search is fundamental to vector databases, retrieval-augmented
generation, similarity search, and memory-based learning. A **k-d tree**
partitions low-dimensional Euclidean space recursively so branches can be
skipped using a geometric lower bound (Bentley, 1975). Unlike approximate
search, this version returns the exact nearest neighbors.

## Algorithm

1. Construct a balanced binary tree by splitting each subset at its median
   point along axis `depth % dimensions`. Coordinates tied at the split
   axis are ordered by their original, zero-based input indices.
2. At each node, calculate squared Euclidean distance to the query and
   maintain a maximum-size `k` heap of the best candidates.
3. Search the side of the splitting plane containing the query first.
4. Search the opposite side whenever the heap is not full **or** the
   squared distance to the splitting plane is no greater than the
   current worst candidate's squared distance.
5. Return `(index, squared_distance)` pairs sorted by increasing
   squared distance, then original index.

The equality check in step 4 matters: equal-distance candidates
can have a **lower index** and must remain eligible.

## Contract

- `kd_tree_knn(points, query, k)` takes a list/tuple of `1..4096`
  feature vectors of equal dimension `1..32`, one matching query vector,
  and a positive integer `k <= number_of_points`.
- Every coordinate must be a finite real (not bool) within
  `[-1_000_000, 1_000_000]`. Empty or ragged data and invalid `k`
  raise `ValueError`.
- Outputs are a list of `(zero_based_index, squared_L2_distance)`.
  Duplicates are valid; ties are resolved by **smallest original index**.
  The input is never mutated.
- Distances are computed with Python binary64. Bounded values and dimensions
  keep squared-distance sums finite for the specified contract.

## Worked example

```python
kd_tree_knn([[0, 0], [3, 4], [1, 0]], [0, 0], 2)
# [(0, 0.0), (2, 1.0)]
```

## Complexity

Median construction sorts each recursive subset: **O(d n log² n)**
time (counting feature-key access), with O(nd + n) retained input/tree
storage. Each query is typically efficient for low dimension,
but **O(nd)** in the worst case due to backtracking. The candidate
heap uses O(k) extra space. This implementation builds the tree once
per function call; a reusable index would amortize that cost.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [kd_tree_knn_python](kd_tree_knn_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests compare results with exhaustive search for multiple queries and
datasets, including duplicates, exact distance ties, low-dimensional
edge cases, and invalid inputs.

## Limitations

K-d trees can be slower than a linear scan in high dimensions, and the
current implementation does not support insertion, deletion, cached
multi-query search, arbitrary metrics, or approximate search. It is an
educational baseline, not a production vector database.

## References

Bentley, J. L. (1975). Multidimensional binary search trees used for
associative searching. *Communications of the ACM, 18*(9), 509–517.
https://doi.org/10.1145/361002.361007
