# Bellman–Ford Shortest Paths

## Why it belongs here

Shortest paths support graph-based retrieval, routing and search.
Unlike Dijkstra's algorithm, **Bellman–Ford** permits negative edge
weights and detects negative-cost cycles reachable from the source.
This makes the contrast between two common shortest-path strategies
concrete (Cormen et al., 2022).

## Algorithm

For a directed graph with `n` vertices and an ordered edge list
`(u, v, weight)`, set `distance[source] = 0` and all other
distances to positive infinity. Repeat up to `n-1` passes over all
edges, applying relaxation when `u` is reachable:

```text
distance[v] = min(distance[v], distance[u] + weight)
```

Terminate early if a whole pass has no updates. Perform one further
pass: an improvement from a reachable node means a reachable
negative-weight cycle, for which finite shortest distances are
undefined and the function fails.

## Contract

- Vertex IDs and the source are **zero-based** in every language,
  including R. The graph has `1 <= n <= 128` vertices and at most
  `16,384` directed edges (duplicates and self-loops are allowed).
- Each edge's `from` and `to` are integer vertex IDs in
  `[0,n-1]`, and the weight is a finite real value in
  `[-1,000,000,1,000,000]`; zero-weight edges are valid.
- Empty edge lists, disconnected components and negative cycles
  **unreachable** from the source are valid. Unreachable nodes
  receive **positive infinity**, never -1 (which is a valid cost).
- All edges are validated, including those in disconnected components.
  Reachable negative cycles cause an error.
- C accepts an array of `BFEdge` structs (or NULL if count is zero)
  and fills a caller-owned length-n output array on success only.
  That output must not overlap the input. Rust takes a slice of
  `Edge` structs and returns `Result<Vec<f64>, &str>`. Python
  takes a list/tuple of three-item edge tuples. R takes a numeric
  matrix with three columns, with one edge per row.
- Floating-point results use standard double precision. Exact
  equality of computed costs is not guaranteed for arbitrary reals.

For example, the edges `[(0,1,4),(0,2,5),(1,2,-2),(2,3,3)]`
with `n=5`, `source=0` give
`[0,4,2,5,+Inf]`. This includes a negative edge but no negative
cycle.

## Complexity

**O(nm)** time for `m` edges (at most `n-1` relaxation rounds
plus a cycle check); **O(n)** auxiliary space for distances.
This is a pedagogical edge-list implementation, not a specialized
sparse-graph or parallel implementation.

## Implementations

| Language | Directory |
| --- | --- |
| C | [bellman_ford_shortest_path_c](bellman_ford_shortest_path_c/) |
| Python | [bellman_ford_shortest_path_python](bellman_ford_shortest_path_python/) |
| Rust | [bellman_ford_shortest_path_rust](bellman_ford_shortest_path_rust/) |
| R | [bellman_ford_shortest_path_r](bellman_ford_shortest_path_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The tests cover negative and zero-cost edges, unreachable vertices,
empty graphs, duplicate edges, a reachable and an unreachable negative
cycle, malformed endpoints, nonfinite weights and oversized inputs.

## Limitations

No path reconstruction, cycle witness, incremental updates or GPU
implementation. Floating-point rounding may complicate negative-cycle
detection for exceptionally small cycle costs; the restricted finite
weight and graph-size contract bounds common arithmetic overflow.

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
  (2022). *Introduction to algorithms* (4th ed.). The MIT Press.
  https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/
