# Single-Link Agglomerative Clustering

## Why it belongs in this repository

Single-link hierarchical clustering is a basic unsupervised method that merges groups whenever two members are particularly close. It can discover elongated clusters without centroids and illustrates the relationship between hierarchical clustering and minimum spanning trees (Hastie et al., 2009). It complements K-means and DBSCAN in the Classical ML collection.

## Algorithm

Given n ordered two-dimensional points and a desired number k of clusters, start with one cluster per input point. Repeatedly choose the **pair of points in different clusters** with minimum squared Euclidean distance

    d(i,j) = (x[i]-x[j])^2 + (y[i]-y[j])^2

and merge their clusters until k remain. Scanning pairs with i < j in ascending order, and updating only on a *strictly smaller* distance, breaks ties by lexicographically smallest pair (i,j). Squared distance preserves the Euclidean ordering without square roots.

Finally relabel clusters 0, 1, ..., k-1 in first-appearance order of the original point sequence. This gives reproducible results for tied coordinates and duplicate points. It is equivalent to single linkage (not complete/average linkage).

## Contract

- Input: flattened row-major coordinates `[x0,y0,x1,y1,...]` for 1 <= n <= 32 two-dimensional points; 1 <= k <= n.
- Every coordinate is an exact integer in [-1000,1000]. Duplicates are allowed; distances use integer arithmetic.
- Output: n zero-based contiguous integer cluster labels, in input order; precisely k distinct labels.
- C: const int32_t pointer and caller-owned size_t label array. On failure (including invalid n, k, pointer or coordinate), the output array is unchanged.
- Python: list or tuple of exact integers; n and k must be plain integers, not booleans. Invalid input raises ValueError.
- Rust: i32 slice; returns None for invalid coordinates/length/k, or Some(Vec<usize>) otherwise.
- R: one-dimensional numeric vector of finite whole coordinates; n and k finite integer-valued scalars. Invalid input raises an error.
- Inputs are not modified.

## Complexity

O((n-k) * n^2) time, at most O(n^3), because each merge scans pairs of points; O(n) auxiliary space. This simple implementation does not build a full dendrogram or precompute a distance matrix.

## Implementations

| Language | Directory |
| --- | --- |
| C | [single_link_clustering_c](single_link_clustering_c/) |
| Python | [single_link_clustering_python](single_link_clustering_python/) |
| Rust | [single_link_clustering_rust](single_link_clustering_rust/) |
| R | [single_link_clustering_r](single_link_clustering_r/) |

## Examples and tests

Four points `[(0,0),(0,1),(10,10),(10,11)]` with k=2 produce `[0,0,1,1]`.

A tied square `[(0,0),(0,2),(2,0),(2,2)]` with k=2 produces `[0,0,0,1]` under the pair-order tie rule. k=n leaves every point in its own cluster; k=1 assigns all points label 0.

Tests cover these cases, duplicate points, singleton, coordinate bounds, 32-point inputs, invalid values and shape validation. Run from the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This implementation handles only two-dimensional **integer** coordinates and a fixed k, not continuous features or arbitrary dimensions. Single linkage can chain through isolated points, unlike centroid-based approaches. The procedure returns a flat partition, not a dendrogram, and is intentionally not optimized for large n.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of statistical learning: Data mining, inference, and prediction* (2nd ed.). Springer. https://doi.org/10.1007/978-0-387-84858-7
