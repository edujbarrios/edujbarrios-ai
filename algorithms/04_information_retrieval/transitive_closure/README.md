# Transitive Closure (Warshall)

## Why it belongs here

A graph's transitive closure answers whether a vertex can reach
another along *any directed path*. This is useful for dependency
analysis, knowledge-graph traversal and reachability queries.
Warshall's dynamic programming builds all-pairs reachability by
considering one intermediate vertex at a time (Cormen et al., 2022).

## Algorithm

Given a directed `n × n` binary adjacency matrix `A` with
`A[u,v]=1` for an edge from `u` to `v`, initialize

```text
reach[u,v] = A[u,v] OR (u == v)
```

For each intermediate vertex `k` (in ascending order), apply:

```text
reach[i,j] = reach[i,j] OR (reach[i,k] AND reach[k,j])
```

The output is **reflexive**: diagonal entries are always 1
because every vertex can reach itself via a zero-length path.
It does not indicate whether the vertex lies on a nontrivial cycle.

Example:

```text
A = [[0,1,0],
     [0,0,1],
     [0,0,0]]
reach = [[1,1,1],
         [0,1,1],
         [0,0,1]]
```

A two-way edge between two vertices produces an all-ones 2×2
closure. Isolated vertices have only a diagonal one.

## Contract

- A square **directed binary** matrix of size `1..64`. Every
  entry must be exactly integer 0 or 1. Self-loops are allowed.
- Result is an `n × n` binary reachability matrix in original
  vertex order, including the reflexive diagonal.
- Invalid shape or any nonbinary entry is rejected; Python
  additionally rejects booleans and nonnumeric entries.
- C accepts row-major `uint8_t[n*n]` with a distinct caller-owned
  output `uint8_t[n*n]`, unchanged on failure. Rust accepts
  `&[u8]` and `n`, returning `Result<Vec<u8>, &str>`.
  Python accepts nested lists/tuples and returns nested lists
  of integers; R accepts a numeric matrix and returns an
  integer-valued matrix.
- No negative, fractional or weighted edges are supported.

## Complexity

Three nested vertex loops: **O(n³)** time and **O(n²)**
auxiliary memory for the output. This dense educational
implementation does not use sparse graph traversal.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [transitive_closure_c](transitive_closure_c/) |
| Python | [transitive_closure_python](transitive_closure_python/) |
| Rust | [transitive_closure_rust](transitive_closure_rust/) |
| R | [transitive_closure_r](transitive_closure_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

The tests cover chains, cycles, self-links, isolated vertices,
invalid dimensions, nonbinary and nonfinite entries.

## Limitations

The output indicates only reachability, not shortest paths,
cycle witnesses, or predecessor paths. With its O(n³) runtime and
dense matrix storage, this reference is not intended for very
large or continuously updated graphs.

## References

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.
  (2022). *Introduction to algorithms* (4th ed.). MIT Press.
  https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/
