# Mean Silhouette Score for Euclidean Clustering

## Why it belongs in AI

The **silhouette coefficient** measures how well an observation belongs
to its assigned cluster relative to other clusters. The mean of these
coefficients gives a practical, label-aware way to evaluate candidate
partitions produced by K-Means, DBSCAN (after handling noise), or other
unsupervised models. It captures both intra-cluster cohesion and
inter-cluster separation (Rousseeuw, 1987).

## Definition

For point `i` in cluster `C(i)`:

```text
a(i) = mean distance(i,j) over j != i in C(i)
b(i) = min over all other nonempty clusters C of
       mean distance(i,j) over j in C

s(i) = (b(i) - a(i)) / max(a(i), b(i))
mean_silhouette = mean_i s(i)
```

The distance is Euclidean. A point in a **singleton cluster** has
silhouette **exactly 0**, even though its own-cluster average would be
undefined. If both `a(i)` and `b(i)` are zero because of duplicate
vectors, define `s(i)=0`. Results are in `[-1,1]`; higher is better.

Example:

```text
points = [[0],[1],[5],[6]]
labels = [0,0,1,1]
individual silhouettes = [9/11, 7/9, 7/9, 9/11]
mean = 79/99 ≈ 0.79797979798
```

## Shared contract

- Input is a **nonempty rectangular numeric matrix** of
  `2 <= n <= 128` observations with `1 <= d <= 32` dimensions.
- All coordinates must be finite real numbers. Every cluster label is
  an **integer in [0,n−1]**, and **at least two distinct labels**
  must be present. Cluster labels need not be contiguous.
- There must be fewer than n distinct clusters; i.e. at least one
  non-singleton is required to make a meaningful partition. This
  reference deliberately rejects partitions with all singleton
  clusters (some libraries also require 2 <= clusters <= n−1).
- Return a finite scalar mean coefficient in [-1,1]. Singleton
  observations contribute 0 to the mean, and duplicate coordinates
  are permitted. No stochastic choices.
- Reject invalid labels, shape mismatch, NaN/Inf, nonrepresentable
  coordinate differences, distances, sums, or averages.
- C takes flat row-major `double points[n*d]` and
  `uint32_t labels[n]`, with a caller-owned `double* result`
  unchanged on failure. Rust also uses flat row-major data and
  returns `Result<f64,...>`. Python accepts nested lists; R accepts
  a numeric matrix. Python disallows bools as numeric values.
- The caller decides how to handle **DBSCAN noise label −1**;
  it is rejected here, not silently treated as a cluster.

## Complexity

For each point, scan all other points and accumulate distances per
cluster: **O(n²d)** time with **O(n)** auxiliary memory (plus input
validation copies in Python). The reference deliberately avoids
a full O(n²) distance matrix.

## Implementations

| Language | Directory |
| --- | --- |
| C | [silhouette_score_c](silhouette_score_c/) |
| Python | [silhouette_score_python](silhouette_score_python/) |
| Rust | [silhouette_score_rust](silhouette_score_rust/) |
| R | [silhouette_score_r](silhouette_score_r/) |

Run tests from the root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the mathematical fixture, mixed singleton clusters,
duplicate coordinates, all-singleton partitions, a single cluster,
non-contiguous IDs, malformed vectors and distance overflow.

## Limitations

This metric is sensitive to feature scaling, dimensionality and
the choice of distance. It does not establish whether a clustering
matches external ground truth. No sample weighting, cosine distance,
per-cluster summary, subsampling or GPU acceleration is provided.
It is a small-data educational reference rather than a replacement
for optimized statistical libraries.

## References

Rousseeuw, P. J. (1987). Silhouettes: A graphical aid to the
interpretation and validation of cluster analysis.
*Journal of Computational and Applied Mathematics, 20*, 53–65.
https://doi.org/10.1016/0377-0427(87)90125-7
