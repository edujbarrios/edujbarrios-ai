# Articulation Points (Tarjan DFS)

## Information retrieval and graph context

An **articulation point** (cut vertex) is a vertex whose removal increases the number of connected components of an undirected graph. These vertices represent structural bottlenecks in networks and are useful for analyzing connectivity in retrieval graphs and infrastructure (Tarjan, 1972).

## Algorithm

Traverse each component with depth-first search, recording discovery time `disc[u]` and the lowest discovery time reachable from the subtree of `u` via tree edges and at most one back edge, `low[u]`.

A nonroot DFS vertex `u` is an articulation point when a DFS child `v` satisfies `low[v] >= disc[u]`. A DFS root is an articulation point when it has more than one DFS tree child. Explore vertex indices in ascending order to produce deterministic results.

## Contract

- Input: `n` vertices, where **1 <= n <= 64**, and a flat **row-major** `n×n` adjacency matrix. Each entry must be the integer **0 or 1**, the diagonal must be all zero (no self-loops), and the matrix must be symmetric (an undirected simple graph). Reject invalid matrices, types, dimensions, loops, and asymmetric edges.
- Output: a strictly ascending vector/list of **zero-based vertex IDs**, without duplicates; return empty for isolated vertices, simple cycles, and graphs without cut vertices.
- C takes `const uint8_t *adjacency`, `size_t n`, a caller-owned `size_t cut_vertices[64]` and `size_t *count_out`. It returns `false` without modifying outputs if validation fails. Python returns `list[int]`; Rust returns `Result<Vec<usize>,&str>`; R returns an integer vector.
- Disconnected graphs are supported, including isolated vertices; removing a vertex from a two-vertex component does not create a cut vertex.
- The input matrix is not modified. Traversal is recursive and bounded by 64 vertices.

## Complexity

A DFS on adjacency lists takes O(V+E). With the specified dense **adjacency matrix**, visiting each vertex scans a row of V entries, so time is **O(V²)** and workspace **O(V)** beyond the caller-provided matrix and result.

## Implementations

| Language | Folder |
| --- | --- |
| C | [articulation_points_c](articulation_points_c/) |
| Python | [articulation_points_python](articulation_points_python/) |
| Rust | [articulation_points_rust](articulation_points_rust/) |
| R | [articulation_points_r](articulation_points_r/) |

## Examples and tests

```text
path: 0—1—2—3—4                -> [1,2,3]
4-cycle: 0—1—2—3—0             -> []
star: 0 connected to 1,2,3,4  -> [0]
disconnected: 0—1—2; 3—4; 5    -> [1]
```

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` in the repository root. Tests include paths, stars, cycles, disconnected graphs, complete graphs, 1/2/64 vertices, and invalid adjacency matrices.

## Limitations

This implementation accepts only unweighted simple **undirected** graphs, not multigraphs, loops or directed networks. A matrix interface makes the overall implementation O(V²) rather than O(V+E). It reports cut vertices, not bridges or biconnected components.

## References

Tarjan, R. (1972). Depth-first search and linear graph algorithms. *SIAM Journal on Computing, 1*(2), 146–160. https://doi.org/10.1137/0201010
