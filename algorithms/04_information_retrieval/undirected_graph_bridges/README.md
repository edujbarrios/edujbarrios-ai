# Undirected Graph Bridges (Tarjan DFS)

## Information retrieval context

A **bridge** (cut edge) is an undirected edge whose removal increases the number of connected components. Bridges reveal fragile connections and bottlenecks in graph-based networks, complementing the separate *articulation points* algorithm for critical vertices (Tarjan, 1972).

## Algorithm

Use depth-first search over each connected component, computing discovery times `disc[u]` and low-link values `low[u]` (the smallest discovery time reachable from u's DFS subtree by tree edges and at most one back edge). For each DFS tree edge `(u,v)`, it is a bridge **if and only if `low[v] > disc[u]`**. A back edge never counts as a bridge. An adjacency matrix is scanned in ascending vertex order.

Return bridges as **flattened endpoint pairs** `[u0,v0,u1,v1,...]`, with `u < v` for each pair, ordered lexicographically by `u` then `v`. For example, the path `0—1—2—3` returns `[0,1,1,2,2,3]`. This flat representation is consistent across all four languages.

## Input/output contract

- Input: a **flat row-major n×n adjacency matrix** of integers **0 or 1** for **1 <= n <= 64**. The graph must be **undirected and simple**: symmetric entries and a zero diagonal; reject self-loops, asymmetric matrices, nonbinary entries, and invalid dimensions/types.
- Output: a flat vector of **zero-based endpoint integers**, two for each bridge in sorted order; the empty vector means no bridges. Isolated vertices, disconnected graphs, cycles and complete graphs are supported.
- C: `bool undirected_graph_bridges(const uint8_t *adjacency, size_t n, size_t endpoints[4032], size_t *bridge_count_out)`. Caller provides space for **4032 endpoints** (more than the maximum possible). The count is the **number of edges**, not the length of the flattened vector; the first `2*bridge_count_out` endpoint entries are valid. Output pointers must be distinct, and outputs remain unchanged on invalid input.
- Python returns `list[int]`, Rust `Result<Vec<usize>,&str>`, R an integer vector; all reject malformed input. Python also rejects boolean/floating-point entries, R logical/complex/matrix/missing values. Inputs are not mutated.
- This is not a minimum spanning tree, shortest-path result, or directed-graph strong bridge definition.

## Complexity

In a dense adjacency matrix the DFS scans at most n rows of length n, and emitting sorted marked pairs scans the upper triangle, yielding **O(n²)** time. Temporary memory is **O(n²)** for bridge markers and O(n) for DFS discovery/low-link state; output uses O(b) for b bridges.

## Implementations

| Language | Folder |
| --- | --- |
| C | [undirected_graph_bridges_c](undirected_graph_bridges_c/) |
| Python | [undirected_graph_bridges_python](undirected_graph_bridges_python/) |
| Rust | [undirected_graph_bridges_rust](undirected_graph_bridges_rust/) |
| R | [undirected_graph_bridges_r](undirected_graph_bridges_r/) |

## Examples and tests

```text
0—1—2—3                        -> [0,1,1,2,2,3]
0—1—2—3—0 (cycle)              -> []
0 connected to 1, 2, 3         -> [0,1,0,2,0,3]
triangle 0—1—2—0 with 2—3—4    -> [2,3,3,4]
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover paths, cycles, stars, mixed cyclic/tree components, isolated vertices, complete graphs, one/two vertices, a 64-vertex path, invalid dimensions and asymmetric/nonbinary graphs.

## Limitations

Only simple unweighted undirected graphs are supported, not parallel edges or loops. The matrix interface consumes O(n²) input memory, even for sparse graphs. The returned endpoint pairs are zero-based, not edge IDs.

## References

Tarjan, R. (1972). Depth-first search and linear graph algorithms. *SIAM Journal on Computing, 1*(2), 146–160. https://doi.org/10.1137/0201010
