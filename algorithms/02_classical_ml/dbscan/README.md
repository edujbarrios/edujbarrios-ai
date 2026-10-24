# DBSCAN Density-Based Clustering

## Why it belongs in AI

Density-Based Spatial Clustering of Applications with Noise (**DBSCAN**)
groups neighboring data points in dense regions and marks isolated
points as noise. Unlike K-Means, it does not require a chosen number of
clusters; unlike a centroid method, it can find nonconvex cluster shapes.
This educational reference implements the classical deterministic
density-expansion procedure (Ester et al., 1996).

## Algorithm

Given `n` feature vectors with `d` coordinates, positive radius
`epsilon`, and positive integer `min_samples`, define an
**inclusive Euclidean neighborhood**:

```text
N_eps(i) = { j | EuclideanDistance(X[i], X[j]) <= epsilon }
core(i)  = |N_eps(i)| >= min_samples   # includes point i itself
```

Visit points in **original row order**. If an unvisited point is not
core, mark it noise initially. If it is core, create a new cluster and
expand it through its neighbors; each **core neighbor** contributes all
its own neighbors to the expansion. An earlier noise point may become
a border point of a later cluster.

Cluster IDs are consecutive **zero-based integers** in discovery order;
noise has label `-1`. When a border point touches multiple clusters,
this particular deterministic scan assigns it to the **first cluster
that reaches it**. Cluster IDs and ambiguous border assignments can
change when input row order changes.

Example:

```text
X = [[0,0],[0.1,0],[0,0.1],[5,5],[5.1,5],[9,9]]
epsilon=0.2; min_samples=2
labels = [0,0,0,1,1,-1]
```

## Shared contract

- A nonempty rectangular `n × d` matrix with `1<=n<=1024`
  and `1<=d<=64`, with only finite real coordinates.
- `epsilon` is a finite real number **strictly greater than zero**;
  `min_samples` is an integer in `[1,n]` and counts the query
  point itself.
- The neighborhood is inclusive of points at exactly `epsilon`.
  Duplicate feature rows are permitted; they count as distinct samples.
- Return one integer label per input row: -1 for noise, 0,1,... for
  clusters. There is no random seed or external clustering dependency.
- All pairwise distances are validated with a stable running
  hypotenuse calculation. Extremely large finite coordinates can
  produce an unrepresentable difference/distance and are rejected.
- C and Rust use **row-major** flat input; Python nested numeric rows;
  R a native numeric matrix. C writes caller-owned `int[n]` output
  **only on success**, with no input/output overlap. Rust returns a
  `Result<Vec<i32>,...>`, Python a list of ints, R an integer vector.
- Python rejects boolean numbers; R rejects invalid types, noninteger
  `min_samples`, empty or nonfinite input matrices.

## Complexity

A dense all-pairs neighborhood matrix uses **O(n²d)** time and
**O(n²)** auxiliary memory; cluster expansion scans at most O(n²)
neighbors. Bounds on n and d keep the educational reference
predictable, but this is not scalable for huge datasets.

## Implementations

| Language | Directory |
| --- | --- |
| C | [dbscan_c](dbscan_c/) |
| Python | [dbscan_python](dbscan_python/) |
| Rust | [dbscan_rust](dbscan_rust/) |
| R | [dbscan_r](dbscan_r/) |

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests exercise multiple clusters and noise, early noise becoming a
border member, identical points, all noise, singletons, zero/invalid
parameters, invalid coordinates and arithmetic overflow.

## Limitations

No spatial index, approximate neighborhoods, GPU kernel, incremental
clustering, automatic epsilon selection, variable-density handling or
HDBSCAN. Border assignments in DBSCAN may be order-dependent.
Distances operate on the raw input feature scale; standardization
should be fitted on training data only when appropriate.

## References

Ester, M., Kriegel, H.-P., Sander, J., & Xu, X. (1996). A density-based
algorithm for discovering clusters in large spatial databases with noise.
In *Proceedings of the Second International Conference on Knowledge
Discovery and Data Mining* (pp. 226–231).
https://www.aaai.org/Papers/KDD/1996/KDD96-037.pdf
