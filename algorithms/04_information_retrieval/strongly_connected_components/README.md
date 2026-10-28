# Strongly Connected Components (Kosaraju)

## Why it belongs here

In a directed graph, vertices belong to the same *strongly connected
component* (SCC) exactly when each can reach the other. SCCs help
condense link graphs, diagnose cyclic dependencies, and analyze
graph-based retrieval and recommendation systems. Kosaraju's two-pass
depth-first search identifies these components (Cormen et al., 2022).

## Algorithm

For an `n × n` binary adjacency matrix `A`, where `A[u,v]=1`
denotes a directed edge from vertex `u` to `v`:

1. Run DFS on the original graph, storing vertices in **finishing order**.
2. Visit vertices in **reverse finishing order**, DFS-traversing
   the *transposed* graph and assigning one component per traversal.
3. Return for each vertex the **minimum vertex index** of its
   component. This makes output labels independent of discovery IDs.

Vertices and neighbors are explored by ascending index. Example:

```text
0 -> 1 -> 2 -> 0, 2 -> 3, 3 <-> 4, 4 -> 5
labels: [0, 0, 0, 3, 3, 5]
```

Here, `{0,1,2}`, `{3,4}` and `{5}` are distinct SCCs.

## Contract

- Square directed adjacency matrix with `1 <= n <= 128`.
- Each cell is exactly 0 or 1; self-loops are allowed.
- Returns a length-`n` zero-based integer label vector in
  original vertex order. Each label is the **minimum vertex ID**
  in the same SCC. Isolated vertices label themselves.
- Reject invalid matrix shape, nonbinary values and nonfinite
  numbers. Python also rejects booleans.
- C accepts row-major `int[n*n]` and writes to caller-owned
  `size_t[n]` only on success; the output must not overlap the
  input. Rust accepts row-major `&[u8]`, `n` and returns
  `Result<Vec<usize>, &str>`. Python takes nested lists/tuples;
  R accepts a numeric matrix and returns an integer vector.

## Complexity

For a dense matrix, the two DFS passes scan up to `n` neighbors
per vertex: **O(n²)** time, **O(n)** auxiliary working memory
(including DFS recursion stacks and labels). No external packages.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [strongly_connected_components_c](strongly_connected_components_c/) |
| Python | [strongly_connected_components_python](strongly_connected_components_python/) |
| Rust | [strongly_connected_components_rust](strongly_connected_components_rust/) |
| R | [strongly_connected_components_r](strongly_connected_components_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include multiple SCCs, disconnected vertices, one-way edges,
strongly connected and acyclic graphs, self-loops, malformed
matrices and nonbinary entries.

## Limitations

This reference returns labels only, not a condensed DAG or traversal
tree. Depth-first recursion is bounded by 128 vertices; the matrix
representation is inefficient for large sparse graphs. This is
not an incremental/dynamic SCC algorithm.

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
  (2022). *Introduction to algorithms* (4th ed.). MIT Press.
  https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/
