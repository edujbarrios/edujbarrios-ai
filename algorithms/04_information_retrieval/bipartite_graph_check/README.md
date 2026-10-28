# Bipartite Graph Check

## Why it belongs here

A bipartite graph permits partitioning vertices into two sets such
that every edge joins opposite sets. This is useful for
user–item recommendation graphs, matching problems, and
network structure analysis. Breadth-first two-coloring recognizes
bipartite graphs and detects odd cycles (Cormen et al., 2022).

## Algorithm

Read an undirected graph as a symmetric binary adjacency matrix.
For each as-yet-uncolored vertex in ascending index order, assign
color 0, then breadth-first traverse its neighbors. Assign each
uncolored neighbor the opposite color, and reject the graph
if an edge joins two vertices of equal color.

A self-loop is an odd cycle of length one and causes rejection.
Disconnected graphs and isolated vertices are supported. If the
graph is bipartite, return **true**; otherwise return **false**.
For the C interface, the boolean return indicates *valid input*,
and the bipartite result is written separately.

## Contract

- Square, symmetric adjacency matrix of size `1..128`.
  Every element must be exactly 0 or 1. Nonfinite, asymmetric,
  nonbinary, or malformed inputs are errors.
- Self-loops, triangles, and other odd cycles are valid **graph
  inputs**, with a bipartite result of false.
- C takes a row-major `uint8_t[n*n]` and a caller-owned `bool *out`.
  It returns false **only for invalid input** and leaves output
  unchanged on failure; input and output cannot overlap.
- Rust takes a row-major `&[u8]` and `n` and returns
  `Result<bool,&str>`. Python accepts nested lists/tuples and
  rejects bool-valued matrix entries. R takes a numeric matrix,
  returns a single logical result and rejects malformed inputs.
- Deterministic BFS begins with each smallest uncolored vertex,
  so a disconnected graph is checked in its entirety.

## Complexity

For the dense representation, **O(n²)** time to validate symmetry
and scan adjacency rows; **O(n)** auxiliary memory for the
color array and BFS queue.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [bipartite_graph_check_c](bipartite_graph_check_c/) |
| Python | [bipartite_graph_check_python](bipartite_graph_check_python/) |
| Rust | [bipartite_graph_check_rust](bipartite_graph_check_rust/) |
| R | [bipartite_graph_check_r](bipartite_graph_check_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The tests cover paths, even cycles, odd cycles, self-loops,
disconnected components with a remote odd cycle, isolated
vertices and invalid adjacency matrices.

## Limitations

Returns only a bipartite decision, not the partition or an
odd-cycle witness. For large sparse graphs, adjacency lists
can reduce the time to O(V+E). No edge weights are supported.

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
  (2022). *Introduction to algorithms* (4th ed.). MIT Press.
  https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/
