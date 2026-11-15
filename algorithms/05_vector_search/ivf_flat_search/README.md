# Simplified IVF-Flat Vector Search

## Why it belongs in AI

Vector databases often organize embeddings into **coarse cells** so queries
only scan a subset of vectors. An inverted-file (IVF) index first assigns
data vectors to coarse representatives, then performs an exact distance
scan within selected posting lists. This illustrates the practical
trade-off between query speed and recall in approximate similarity
search (Jégou et al., 2011).

## Algorithm

This small from-scratch variant uses **deterministic farthest-first
coarse representatives**, rather than iterative k-means training:

1. Take the first point as the initial centroid. Add the point whose
   minimum squared Euclidean distance to an existing centroid is
   greatest, breaking ties by the lowest original index.
2. Assign every point to the closest centroid (lowest centroid index
   on exact distance ties), forming inverted posting lists.
3. At query time, choose the `n_probe` centroids nearest the query,
   breaking ties by centroid order.
4. Scan **all original vectors** in those posting lists; return
   `(original_index, squared_distance)` pairs sorted by distance
   then original input index, truncated to at most `k`.

With `n_probe == n_clusters`, every point is visited and the
result is exact. Otherwise, **recall is not guaranteed**. If fewer
than `k` points are in the probed lists, fewer than `k`
results are returned.

## Contract

- `ivf_flat_search(points, query, n_clusters, n_probe, k)`
  takes `1..4096` equal-length coordinate vectors of dimension
  `1..32` and one matching query vector.
- Coordinates must be finite non-boolean reals with absolute value
  at most 1,000,000. Duplicate data points are allowed.
- `n_clusters` is an integer in `1..number_of_points`,
  but the dataset must contain **at least n_clusters distinct
  feature vectors**. `n_probe` is in `1..n_clusters`,
  `k` in `1..number_of_points` (all must be non-boolean integers).
- Outputs are `(zero_based_index, squared_L2_distance)`
  pairs in ascending distance/index order, possibly fewer than `k`.
  Invalid inputs raise `ValueError`; inputs are never modified.
- Rebuilding occurs on every call. Binary64 arithmetic and bounded
  coordinates keep the squared-distance sums finite.

## Worked example

```python
points = [[0], [1], [9], [10]]
ivf_flat_search(points, [9.4], n_clusters=2, n_probe=1, k=3)
# [(2, 0.16000000000000028), (3, 0.3599999999999996)]
ivf_flat_search(points, [9.4], n_clusters=2, n_probe=2, k=3)
# indexes [2, 3, 1] in nearest-neighbor order
```

The first centroid is `[0]`; farthest-first chooses `[10]`.
A one-cell query searches only the two vectors in that cell.

## Complexity

For `n` vectors, `d` dimensions, `c` clusters, and
`r` candidates in probed posting lists: **O(ncd + cd + r log r)**
time per build-and-query call, including greedy coarse selection,
assignment and final sorting. Copied vectors, posting lists and
centroid indices require **O(nd + n + c)** space. A reusable index
could amortize construction over many queries.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [ivf_flat_search_python](ivf_flat_search_python/) |

Other languages are not yet implemented.

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover one-cell approximate retrieval, exhaustive full-probe
equivalence, duplicates, tie policies, invalid parameters and
input immutability.

## Limitations

This is not a production IVF implementation: it uses farthest-first
seeds without k-means refinement, does not persist an index, and
supports only dense Euclidean vectors. It reports **squared L2
distance**, not cosine similarity. Small `n_probe` can miss true
neighbors. No measured recall–latency benchmarking is claimed.

## References

Jégou, H., Douze, M., & Schmid, C. (2011). Product quantization for
nearest neighbor search. *IEEE Transactions on Pattern Analysis and
Machine Intelligence, 33*(1), 117–128.
https://doi.org/10.1109/TPAMI.2010.57
