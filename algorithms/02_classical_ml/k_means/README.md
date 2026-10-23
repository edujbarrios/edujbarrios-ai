# K-Means Clustering (Lloyd's Algorithm)

## Why it belongs in the AI hub

K-means is a core **unsupervised learning** method that groups numerical
observations around representative centroids. It introduces iterative
optimization, hard assignments and centroid recomputation—ideas used in
vector quantization, clustering embeddings and retrieval preprocessing.
The classic least-squares quantization formulation is described by
Lloyd (1982).

## Algorithm

For `n` samples of `d` features, choose `k` centroids and repeat:

1. Assign each sample to its **nearest centroid** by Euclidean distance.
2. Recompute each nonempty cluster's centroid as the arithmetic mean of
   its assigned samples.
3. Stop when assignments do not change or after `max_iterations` updates.

**Deterministic initialization:** use the **first k sample points** as
centroids. These first k points must be pairwise distinct. Distances tied
exactly between centroids are assigned to the **lower cluster index**.
If a cluster becomes empty, retain its previous centroid.

No randomness or dataset-dependent seed search is performed.

## Shared contract

- Inputs: a nonempty **rectangular** matrix of finite real samples, with
  `n >= k >= 1`, `d >= 1`, and a **positive integer** iteration limit.
- Return **k centroids** in the same feature dimensionality and one
  **zero-based cluster assignment** per original sample.
- An empty cluster retains its previous centroid and produces no error.
  The first k data points must be pairwise distinct, including after exact
  floating-point comparison.
- Invalid dimensionality, NaN, infinity, duplicate initial centroids,
  non-finite distances or non-finite centroid sums produce an error.
- C and Rust use a **row-major flat array** plus `n`, `d`, `k`, and
  `max_iterations`. Python accepts nested numeric rows; R takes a base
  matrix. Python and R return centroids and assignments in a container.
- C returns `bool` and only writes caller-owned centroid/assignment arrays
  on success. The caller must provide input of at least `n*d` doubles,
  centroid output of `k*d` doubles and labels output of `n` indices.
  These buffers must not overlap.
- Python/R raise errors; Rust returns `Result<(...), ...>`.
  Cluster identifiers are **zero-based in all languages**, including R.
- Euclidean distances are accumulated with a numerically stable hypotenuse
  recurrence. Centroid sums use ordinary floating-point addition, and may
  overflow on extreme inputs even when a mathematical mean exists;
  such inputs are rejected rather than silently producing a wrong result.
- This version **does not guarantee a global optimum**, even if convergence
  is detected. Report initial-centroid policy and iteration limits when
  comparing with other implementations.

## Worked example

```text
samples = [[0, 0], [10, 10], [0, 2], [10, 12]]
k = 2, max_iterations = 10
initial centroids = [[0, 0], [10, 10]]
final centroids   = [[0, 1], [10, 11]]
labels (zero-based) = [0, 1, 0, 1]
```

## Complexity

At most `max_iterations` iterations of assignment and update:

- **Time:** `O(max_iterations × n × k × d)`.
- **Auxiliary memory:** `O(n + k*d + k)` for labels, centroid
  accumulators, counts and centroid copies.

## Implementations

| Language | Folder |
| --- | --- |
| C | [k_means_c](k_means_c/) |
| Python | [k_means_python](k_means_python/) |
| Rust | [k_means_rust](k_means_rust/) |
| R | [k_means_r](k_means_r/) |

## Testing

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from the repository root.
Tests cover the known two-cluster result, one-cluster averaging, invalid
dimensions and counts, duplicate seeds, non-finite inputs and overflowing
intermediates. CUDA is not included, because parallel performance claims
would require independent GPU verification.

## Limitations

No k-means++ seeding, multiple restarts, mini-batches, standardized features,
weighted samples, advanced convergence tolerance or empty-cluster reseeding.
The algorithm can converge to a local minimum; its results depend strongly
on the first k samples and the selected iteration budget. This is a
transparent educational implementation, not a production clustering engine.

## References

Lloyd, S. (1982). Least squares quantization in PCM. *IEEE Transactions on
Information Theory, 28*(2), 129–137.
https://doi.org/10.1109/TIT.1982.1056489
