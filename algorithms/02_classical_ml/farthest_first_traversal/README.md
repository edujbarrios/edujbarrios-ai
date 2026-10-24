# Farthest-First Traversal for K-Center Selection

## Why it belongs in AI

Selecting representative, well-separated points is useful for
**clustering**, prototype selection and approximation methods.
Gonzalez's farthest-first traversal is a greedy method for the
metric **k-center** problem: after picking one initial center,
repeatedly choose the point with the largest distance to its nearest
already-chosen center (Gonzalez, 1985).

This algorithm **selects data-point indices as centers**. It does not
assign clusters or refine centroids as K-Means does. The classical
2-approximation guarantee assumes a valid metric, exact distances
and a discrete k-center objective.

## Algorithm

Given a finite real `n × d` matrix `X` and `1<=k<=n`:

```text
selected = [0]                           # deterministic first center
nearest[i] = distance(X[i], X[0])
while len(selected) < k:
    next = unselected i maximizing nearest[i]
    # ties broken by smallest original zero-based index
    append next
    nearest[i] = min(nearest[i], distance(X[i], X[next]))
return selected
```

Euclidean distance is computed with an iterative **hypotenuse**
recurrence to avoid avoidable intermediate square overflow.
The algorithm maintains nearest distances incrementally, rather than
recomputing them from scratch for every selected center.

Example:

```text
points = [[0,0],[1,0],[9,0],[10,0]]
k = 3
selected = [0,3,1]

step 1: start at index 0
step 2: index 3 is 10 units away
step 3: indices 1 and 2 are each 1 unit from their nearest center;
        index 1 wins the tie
```

## Shared contract

- Input is a **nonempty** matrix of `n>=1` rows and `d>=1`
  finite real coordinates; `k` is an integer in `[1,n]`.
- Return `k` **distinct zero-based indices** in the greedy order
  selected, not sorted order.
- The first center is always row index 0. For tied max-min distances,
  choose the smallest currently unselected index. Identical vectors
  and duplicate rows are permitted; if all remaining distances are
  zero, the lowest unselected index is chosen.
- Reject NaN/Inf, malformed dimensions, wrong cutoff and **nonfinite
  coordinate differences or distances**. No near-tie tolerance:
  exact float comparison is used for selection. This is a finite
  double-precision educational reference.
- C takes flat row-major `points[n*d]` and caller-owned
  `indices[k]`. Failure leaves indices unmodified; caller provides
  sufficient nonoverlapping buffers.
  Rust takes row-major flat data and returns `Result<Vec<usize>,...>`.
  Python takes nested rows and returns an integer list.
  R takes a native numeric matrix and returns zero-based integers.
- No random seed is needed because this method is deterministic.
  It is **not K-Means++**, which samples subsequent centers
  probabilistically using squared distances.

## Complexity

**O(k*n*d)** time: after each new center, update all n nearest-center
distances in O(n*d). O(n+k) scratch storage and output (Python
additionally copies the input matrix in validation).

## Implementations

| Language | Directory |
| --- | --- |
| C | [farthest_first_traversal_c](farthest_first_traversal_c/) |
| Python | [farthest_first_traversal_python](farthest_first_traversal_python/) |
| Rust | [farthest_first_traversal_rust](farthest_first_traversal_rust/) |
| R | [farthest_first_traversal_r](farthest_first_traversal_r/) |

Run the strict CPU test suites:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover center selection, equal-distance ties, duplicate rows,
single-center requests, all-selected cases, invalid inputs and
nonrepresentable Euclidean differences.

## Limitations

Exact scan over all input points for each selection; unsuitable for
very large data without spatial indexes or parallel execution.
No K-Means centroid recomputation, cluster assignment, probabilistic
K-Means++ sampling, streaming coresets or GPU acceleration. The
approximation guarantee does not extend automatically to arbitrary
similarity functions or to K-Means' sum-of-squared-errors objective.

## References

Gonzalez, T. F. (1985). Clustering to minimize the maximum intercluster
distance. *Theoretical Computer Science, 38*, 293–306.
https://doi.org/10.1016/0304-3975(85)90224-5
