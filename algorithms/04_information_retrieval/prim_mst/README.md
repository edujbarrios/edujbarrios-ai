# Prim Minimum Spanning Tree

## Why it belongs here

Prim's algorithm is a greedy method for finding a minimum-cost set
of edges connecting all vertices of an undirected weighted graph.
Together with Kruskal's algorithm, it demonstrates two different
approaches to minimum spanning trees for graph retrieval, clustering
and network design (Prim, 1957).

## Algorithm

Represent the graph by a **symmetric** `n × n` adjacency matrix
`W`. Positive entries are edge weights and zero means *no edge*
(except for the required zero diagonal). Starting from vertex 0:

1. Maintain the smallest edge cost `best[v]` to each vertex outside
   the growing tree. Set `best[0]=0` and others to infinity.
2. Choose the outside vertex with the smallest finite `best[v]`,
   breaking ties by lowest vertex index. Add that edge's cost to the
   total.
3. Update every outside neighbor: `best[v]=min(best[v], W[u,v])`
   for positive weights.
4. Repeat until all vertices have been visited. If no finite
   candidate exists, the graph is disconnected and the function fails.

This reference returns the **total tree weight**, not the chosen
edge list. For example:

```text
W = [[0,1,4,0],
     [1,0,2,5],
     [4,2,0,3],
     [0,5,3,0]]
MST total weight = 6    (edges cost 1 + 2 + 3)
```

## Contract

- Accept a square undirected graph with `1 <= n <= 128`.
- Weights must be finite in `[0,1,000,000]`; **zero means no edge**
  and therefore a valid zero-weight edge cannot be represented.
  The diagonal must be zero; all mirrored entries must match exactly.
- All off-diagonal positive weights form undirected edges.
  Self-loops are excluded by the diagonal rule.
- The graph must be **connected** if `n>1`; otherwise the result
  is undefined and an error is returned. The one-vertex graph returns 0.
- C takes row-major `double[n*n]` and a caller-owned output pointer,
  leaving it unchanged on error (the output cannot overlap the input).
  Rust takes a row-major `&[f64]` and explicit `n`, returning
  `Result<f64, &str>`. Python takes a nested list or tuple of rows.
  R takes a numeric matrix. Python rejects bool entries.
- The sum is computed in double precision. Ties are resolved
  deterministically by the lowest vertex index, though only the total
  weight is exposed.

## Complexity

Dense minimum-selection and neighbor scans use **O(n²)** time and
**O(n)** auxiliary memory. No external dependencies or priority
queues are used.

## Implementations and tests

| Language | Directory |
| --- | --- |
| C | [prim_mst_c](prim_mst_c/) |
| Python | [prim_mst_python](prim_mst_python/) |
| Rust | [prim_mst_rust](prim_mst_rust/) |
| R | [prim_mst_r](prim_mst_r/) |

Run:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include a four-node reference graph, ties, a singleton,
disconnected components, non-square and asymmetric matrices,
nonzero diagonal, invalid bounds and nonfinite values.

## Limitations

Only undirected, connected, positive-weight graphs are supported.
Explicit zero-weight and negative-weight edges, edge recovery, sparse
adjacency lists, dynamic updates, large graphs and GPU execution are
out of scope for this compact reference. Numerical totals have
ordinary floating-point rounding error.

## References

- Prim, R. C. (1957). Shortest connection networks and some
  generalizations. *Bell System Technical Journal, 36*(6), 1389–1401.
  https://doi.org/10.1002/j.1538-7305.1957.tb01515.x
