# Breadth-First Search Shortest Paths

## Why it belongs here

Breadth-first search (BFS) visits a graph layer by layer, making it
a fundamental graph-traversal primitive for retrieval, routing,
link analysis and nearest-hop discovery. In an **unweighted** graph,
the first discovered distance is the minimum possible number of
edges from the source (Cormen et al., 2022).

## Algorithm

Use an `n × n` adjacency matrix for a directed graph:
`W[u,v] = 1` indicates an edge `u -> v`; `0` means no edge.
Self-loops are permitted but do not decrease any distance.

Initialize `distance[source]=0`; all other distances are `-1`.
Push the source into a first-in-first-out queue. For each dequeued
vertex `u`, visit its neighbors in ascending vertex-index order.
Whenever `W[u,v]=1` and `v` is undiscovered, set
`distance[v]=distance[u]+1` and enqueue `v`.

Example:

```text
W = [[0,1,1,0,0,0],
     [0,0,0,1,0,0],
     [0,0,0,1,0,0],
     [0,0,0,0,1,0],
     [0,0,0,0,0,0],
     [0,0,0,0,0,0]]
source = 0
distances = [0,1,1,2,3,-1]
```

## Contract

- Square directed adjacency matrix with `1 <= n <= 128`.
- Every entry is exactly integer 0 or 1. A zero diagonal is
  conventional but self-links are allowed.
- The source is a **zero-based** integer in `[0,n-1]`, including R.
- Return an n-element vector of integer hop distances in original
  vertex order, with `-1` for unreachable nodes.
- Reject malformed matrices, invalid source and nonbinary entries
  (including NaN, infinity, negative weights, and fractions).
  Python additionally rejects boolean values.
- C accepts row-major `int[n*n]`, writes to a caller-owned
  `int[n]` only on success (which must not overlap the input).
  Rust accepts row-major `&[u8]` and returns
  `Result<Vec<i32>, &str>`. Python accepts nested lists/tuples.
  R accepts a numeric matrix and returns an integer vector.

## Complexity

Dense implementation: **O(n²)** time for row scans, **O(n)**
additional memory for distances and queue, regardless of edge count.
This is not a sparse adjacency-list implementation.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [bfs_shortest_path_c](bfs_shortest_path_c/) |
| Python | [bfs_shortest_path_python](bfs_shortest_path_python/) |
| Rust | [bfs_shortest_path_rust](bfs_shortest_path_rust/) |
| R | [bfs_shortest_path_r](bfs_shortest_path_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The tests include a multihop route, equally short alternatives,
unreachable nodes, cycles, self-loops, one-node graphs, and
invalid dimensions, sources and matrix values.

## Limitations

BFS minimizes **hop count**, not accumulated nonuniform edge cost.
It cannot directly solve weighted shortest paths, return
predecessor routes or run over streaming/dynamic graphs.
Use Dijkstra or Bellman–Ford from this catalogue when edge
weights matter.

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
  (2022). *Introduction to algorithms* (4th ed.). The MIT Press.
  https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/
