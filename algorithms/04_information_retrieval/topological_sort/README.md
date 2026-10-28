# Topological Sort (Kahn's Algorithm)

## Why it belongs here

A topological ordering places each vertex of a directed acyclic
graph before all vertices it points to. Such orders are useful for
dependency planning, computational graphs, inference pipelines,
and graph-processing tasks. This implementation uses the
indegree-deletion approach described in standard graph algorithm
texts (Cormen et al., 2022).

## Algorithm

Use an `n × n` adjacency matrix `W`, with `W[u,v]=1` meaning a
directed edge `u -> v`, and `0` meaning no edge.

1. Count each vertex's incoming edges (`indegree[v]`).
2. Repeatedly select the **lowest-index unused vertex with indegree 0**.
3. Append the vertex to the output, then decrement the indegrees of
   all of its outgoing neighbors.
4. If there is no eligible vertex before all `n` vertices are
   emitted, the graph contains a directed cycle and is rejected.

Example:

```text
W = [[0,1,1,0],
     [0,0,0,1],
     [0,0,0,1],
     [0,0,0,0]]
order = [0,1,2,3]
```

Other valid topological orders may exist; the minimum-index choice
makes this result deterministic. A zero-edge graph is ordered as
`[0,1,...,n-1]`.

## Contract

- Directed adjacency matrix with `1 <= n <= 128`.
  All entries must be exactly 0 or 1.
- **Zero-based** vertex identifiers across all four languages.
- A self-loop is a cycle; any directed cycle raises an error.
  Disconnected DAG components are accepted.
- C accepts a row-major `int[n*n]` matrix, writes a caller-owned
  `size_t[n]` result only on success and requires the output to
  not overlap the input. Rust accepts `&[u8]` and explicit `n`,
  returning `Result<Vec<usize>, &str>`. Python takes nested
  lists/tuples (rejecting bool values). R takes a numeric matrix
  and returns an integer vector.
- All matrix entries must be valid, including entries in components
  disconnected from vertex 0.

## Complexity

Dense indegree computation, min-index selection and outgoing scans:
**O(n²)** time and **O(n)** auxiliary space. A sparse queue
implementation can be more efficient on large graphs but is not
needed for this bounded educational example.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [topological_sort_c](topological_sort_c/) |
| Python | [topological_sort_python](topological_sort_python/) |
| Rust | [topological_sort_rust](topological_sort_rust/) |
| R | [topological_sort_r](topological_sort_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include a fork-and-join DAG, disconnected vertices,
index-based tie resolution, cycles and self-loops, a singleton
graph, malformed dimensions and nonbinary edge entries.

## Limitations

Returns one valid ordering, not all possible orderings.
It provides no diagnostic cycle path, incremental sorting,
weighted scheduling or sparse graph representation.

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
  (2022). *Introduction to algorithms* (4th ed.). The MIT Press.
  https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/
