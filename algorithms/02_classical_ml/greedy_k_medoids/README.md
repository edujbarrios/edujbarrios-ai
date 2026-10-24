# Greedy K-Medoids (BUILD Initialization)

## Why it belongs in AI

A **medoid** is a real member of a dataset selected to represent
other observations. Unlike a K-Means centroid, it is never an
artificial averaged point. Greedy K-medoids selection supports
representative selection, robust clustering initialization and
facility-location intuition (Kaufman & Rousseeuw, 1990).

This reference implements a deterministic **BUILD-style greedy
selection**, and explicitly **does not** perform the SWAP phase of
Partitioning Around Medoids (PAM). It does not promise to find the
global optimal k-medoid subset.

## Algorithm

For n finite feature vectors and desired k medoids, precompute
pairwise Euclidean distances `D[i,j]`.

```text
first = argmin_c sum_i D[i,c]  # ties by lowest index
selected = [first]
nearest[i] = D[i,first]

while len(selected) < k:
    next = argmin_{c not selected} sum_i min(nearest[i],D[i,c])
    # equivalently maximize reduction in total nearest-medoid cost
    # exact cost ties -> smallest original index
    selected.append(next)
    nearest[i] = min(nearest[i],D[i,next])

return selected
```

The method greedily reduces `sum_i min_{c in selected} D[i,c]`,
the total distance from each point to its nearest selected medoid.
It chooses the next representative using total cost, not the
maximum nearest distance (the separate farthest-first heuristic).

Example with one-dimensional coordinates:

```text
X = [[0],[1],[9],[10]]

k=1 -> [1]       # indices 1 and 2 tie as global medoid; choose 1
k=2 -> [1,2]     # indices 2 and 3 tie in improved cost; choose 2
k=3 -> [1,2,0]   # indices 0 and 3 tie; choose 0
```

## Shared contract

- Input nonempty finite-real matrix `X[n,d]` with
  **1<=n<=128** and **1<=d<=32**; integer `k∈[1,n]`.
- Return **k distinct zero-based original row indices** in selection
  order, not sorted order. Equal/duplicate vectors are permitted,
  but are still different candidate rows.
- Every candidate and pairwise distance is validated. Use stable
  running `hypot` arithmetic to avoid unnecessary square overflow.
  If a coordinate difference, distance or accumulated objective
  becomes nonfinite, reject the input.
- C and Rust accept flat **row-major** matrices; Python accepts a
  nested list/tuple of rows; R accepts a numeric matrix. C returns
  a Boolean and writes caller-owned `size_t medoids[k]` **only
  on complete success**, with correctly sized, nonoverlapping
  arrays. Rust returns `Result<Vec<usize>,...>`.
- Exact ties choose the smallest currently eligible index. There
  is no random initialization or sampling. Python rejects bool
  coordinates and k; R requires a finite integer-valued k.

## Complexity

Compute the full distance matrix in **O(n²d)** time and O(n²)
memory. Each new medoid selection scans n unselected candidates and
up to n data points: **O(k n²)** additional time, with O(n)
nearest-distance working storage.

## Implementations

| Language | Directory |
| --- | --- |
| C | [greedy_k_medoids_c](greedy_k_medoids_c/) |
| Python | [greedy_k_medoids_python](greedy_k_medoids_python/) |
| Rust | [greedy_k_medoids_rust](greedy_k_medoids_rust/) |
| R | [greedy_k_medoids_r](greedy_k_medoids_r/) |

Run from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the worked example, equal vectors, singleton selection,
all-medoid output, invalid k/dimensions, nonfinite features and
nonrepresentable distances.

## Limitations

This is greedy BUILD initialization only; **no PAM medoid swaps,
reassignment output, sampling shortcuts or convergence loop**.
It is intended for small datasets because the distance matrix is
quadratic. On adversarial data, greedy decisions may be suboptimal.
No GPU implementation or scalability claims.

## References

Kaufman, L., & Rousseeuw, P. J. (1990). *Finding groups in data:
An introduction to cluster analysis*. Wiley.
https://doi.org/10.1002/9780470316801
