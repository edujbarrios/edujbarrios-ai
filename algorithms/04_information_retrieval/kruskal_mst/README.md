# Kruskal Minimum Spanning Tree

## Motivation

**Kruskal's algorithm** finds a minimum-weight set of edges that
connects every vertex in an undirected graph without cycles.
It demonstrates a practical greedy strategy and the
**disjoint-set union (union-find)** data structure.
Minimum spanning trees are useful for similarity graphs, graph
summarization, network design, and hierarchical clustering
(Kruskal, 1956).

This reference returns a single *minimum spanning tree (MST)*.
Disconnected graphs are rejected rather than returning a minimum
spanning forest without explaining that difference.

## Algorithm

From symmetric weighted adjacency `W[n,n]`, where `+Infinity`
represents an absent edge:

1. Extract the finite upper-triangular edges `(u,v,W[u,v])`, `u<v`.
2. Sort edges by **ascending weight**, then **ascending u**, then
   **ascending v** for deterministic ties.
3. Make each vertex its own disjoint set.
4. Scan edges in order; if their endpoints are currently in
   different components, union the components and append that edge.
5. Stop after `n-1` chosen edges; reject if the graph cannot be
   connected.

Example:

```text
W = [[0,   1,   4, Inf],
     [1,   0,   2,   5],
     [4,   2,   0,   3],
     [Inf, 5,   3,   0]]

Selected edges (in greedy order):
[(0,1), (1,2), (2,3)]
Total edge weight = 6
```

The selected vertices are **zero-based** and each returned edge has
`u<v`. The selected edge list need not be ordered by endpoint.

## Shared contract

- Input is a **nonempty symmetric n×n** matrix, `1<=n<=64`.
- `W[i,i]=0`, and every off-diagonal entry is either a finite
  nonnegative weight in `[0,1,000,000]` or `+Infinity`
  meaning *no edge*. **Negative weights, NaN and -Infinity are
  rejected**; nonzero diagonal and asymmetry are also rejected.
- An undirected edge appears in both symmetric matrix positions.
  An edge of weight **0 is allowed**. Exact equality is required
  for matrix symmetry, not a numeric tolerance.
- For a connected graph return exactly `n−1` undirected edges
  in selection order and their finite total weight. For `n=1`,
  return an empty edge set and total cost 0.
- If a graph is disconnected or input invalid, return an error.
  The algorithm will never silently return a partial tree.
- C/Rust matrices are flat **row-major**; Python nested lists;
  R a native numeric matrix. C uses caller-owned `size_t
  from[n-1], to[n-1]` and `double* total`; for `n=1`,
  the edge arrays may be NULL. C outputs stay **unmodified
  on failure** and must be nonoverlapping with input and each
  other. Rust returns `Result<(Vec<(usize,usize)>,f64),...>`,
  Python returns a pair, and R returns a list with an integer
  two-column matrix of selected endpoints and the total.
- Python rejects boolean entries. Floating-point sums must remain
  finite; total cost is computed from the exact input weights.

## Complexity

Extract O(n²) edges, sort up to n(n−1)/2 of them in
**O(n² log n)** time and O(n²) memory. DSU union-by-size plus
path compression makes the edge scans effectively linear.

## Implementations

| Language | Directory |
| --- | --- |
| C | [kruskal_mst_c](kruskal_mst_c/) |
| Python | [kruskal_mst_python](kruskal_mst_python/) |
| Rust | [kruskal_mst_rust](kruskal_mst_rust/) |
| R | [kruskal_mst_r](kruskal_mst_r/) |

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover a weighted MST, exact equal-weight ties, zero edges,
singleton graphs, disconnected graphs, asymmetric inputs,
negative weights, NaNs and invalid diagonals.

## Limitations

Finite dense undirected weighted graphs only; no directed
arborescences, minimum spanning forest output, online graph
updates, graph embedding model, GPU kernel, large-graph streaming
or specialized sparse-graph representation.

## Reference

Kruskal, J. B. (1956). On the shortest spanning subtree of a graph
and the traveling salesman problem. *Proceedings of the American
Mathematical Society, 7*(1), 48–50.
https://doi.org/10.1090/S0002-9939-1956-0078686-7
