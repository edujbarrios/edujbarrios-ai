# Undirected Connected Components

## Why it belongs here

Connectivity groups vertices that can reach one another along
undirected edges. Components can identify isolated subnetworks
in a citation, similarity or recommendation graph. This
reference uses breadth-first traversal (Cormen et al., 2022);
it complements the directed strongly connected components
implementation already in the catalogue.

## Algorithm

Given a symmetric binary adjacency matrix `A` of size `n × n`,
visit vertices in ascending numerical order. For each unseen vertex
`s`, run breadth-first search over its neighbors, marking every
vertex reached with label `s`. Because the first unseen vertex
in a component is its smallest index, each component label is
its **minimum zero-based vertex index**.

Example:

```text
0 -- 2 -- 3      1 -- 4       5
labels = [0, 1, 0, 0, 1, 5]
```

Self-loops do not affect connectivity. An empty edge set
produces labels `[0,1,...,n-1]`.

## Contract

- A square **symmetric 0/1 adjacency matrix**, `1 <= n <= 128`.
- Each element must be exactly 0 or 1. Asymmetric matrices and
  nonbinary entries are errors. Self-loops are allowed.
- Return an integer vector of `n` minimum-index component labels.
  All labels are zero-based, including the R implementation.
- C uses a row-major `uint8_t[n*n]` input and a caller-owned
  `size_t[n]` output, unchanged on error and not overlapping
  the input. Rust uses a row-major `&[u8]` and returns
  `Result<Vec<usize>, &str>`. Python accepts nested lists or
  tuples but rejects booleans. R uses a numeric matrix and
  returns an integer vector.
- Disconnected and one-vertex graphs are valid.

## Complexity

Scanning the dense rows of all discovered vertices takes
**O(n²)** time. The queue, labels and visited state require
**O(n)** auxiliary memory.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [undirected_connected_components_c](undirected_connected_components_c/) |
| Python | [undirected_connected_components_python](undirected_connected_components_python/) |
| Rust | [undirected_connected_components_rust](undirected_connected_components_rust/) |
| R | [undirected_connected_components_r](undirected_connected_components_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover multiple components, connected/edgeless graphs,
self-loops, singletons, asymmetric matrices, nonbinary entries,
and invalid matrix dimensions.

## Limitations

This is not a directed SCC algorithm or an incremental union-find
data structure. For larger sparse graphs, adjacency lists
would typically use less time and space.

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
  (2022). *Introduction to algorithms* (4th ed.). MIT Press.
  https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/
