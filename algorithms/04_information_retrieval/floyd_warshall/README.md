# Floyd–Warshall All-Pairs Shortest Paths

## AI motivation

**Floyd–Warshall** is a dynamic-programming algorithm for shortest
paths between *every pair* of vertices in a weighted graph. It is
a useful building block for knowledge-graph analysis, routing,
similarity graph preprocessing, and understanding graph-based
search. Unlike Dijkstra's algorithm, it supports **negative edges**
provided there is **no negative-weight cycle** (Floyd, 1962).

This is an exact, small-graph reference over double-precision edge
weights. It does not implement path reconstruction.

## Recurrence

Represent a directed graph by `W[i,j]`: a finite edge weight or
`+Infinity` when no direct edge exists. Require `W[i,i]=0`
and `W[i,j] >= -1,000,000` for finite entries.

Initially `D=W`. For intermediate vertex `k`, update:

```text
D[i,j] = min(D[i,j], D[i,k] + D[k,j])
```

Only sum finite distances. `+Infinity` means unreachable and
remains an allowed output. After all updates, any `D[i,i]<0`
indicates a **negative cycle**, and the algorithm rejects the graph
rather than returning ambiguous negative-cycle distances.

Example:

```text
W = [[0,    3, Inf],
     [Inf,  0,  -2],
     [4,  Inf,   0]]

D = [[0,   3,  1],
     [2,   0, -2],
     [4,   7,  0]]
```

## Shared contract

- A nonempty **n×n** adjacency matrix, `1<=n<=64`.
  Valid entries are finite real weights in **[-1e6,1e6]** or
  positive infinity to denote **an absent edge**. Negative infinity
  and NaN are forbidden.
- Every diagonal element **must equal zero**. Self-loops of nonzero
  cost and explicit negative self-loops are rejected rather than
  ignored.
- Return a **new n×n distance matrix** in original vertex order,
  using `+Inf` for unreachable pairs. Negative edges are allowed,
  but a negative cycle anywhere in the graph is rejected.
- Reject unrepresentable distance additions or intermediate values.
  Since `n<=64` and each finite edge magnitude is <=1e6,
  additions are comfortably inside double precision, yet checked.
- C/Rust receive **flat row-major** matrices, C writes a distinct
  caller-owned output buffer `distances[n*n]` **only on success**.
  Python uses nested rows and returns nested output; R accepts a
  numeric matrix and returns a matrix. Rust returns
  `Result<Vec<f64>,...>`, Python and R raise errors.
- C input/output buffers have adequate capacity and **must not
  alias**. Python rejects bool entries. All implementations treat
  positive infinity as a deliberate *missing edge*, not numeric
  corruption.

## Complexity

**O(n³)** time and **O(n²)** output/storage. No randomization,
negative-edge preprocessing or external graph package.

## Implementations

| Language | Path |
| --- | --- |
| C | [floyd_warshall_c](floyd_warshall_c/) |
| Python | [floyd_warshall_python](floyd_warshall_python/) |
| Rust | [floyd_warshall_rust](floyd_warshall_rust/) |
| R | [floyd_warshall_r](floyd_warshall_r/) |

Run from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the worked example, disconnected graphs, singleton
graphs, negative cycles, bad diagonals, invalid finite weights,
NaN and negative infinity.

## Limitations

There is no predecessor/path reconstruction, sparse-graph
acceleration, dynamic graph updates, GPU kernel, or guaranteed
behavior on unrepresentably large arbitrary input weights
(which are rejected by the input contract). This algorithm
computes distances, not a learned representation or ranking model.

## Reference

Floyd, R. W. (1962). Algorithm 97: Shortest path.
*Communications of the ACM, 5*(6), 345.
https://doi.org/10.1145/367766.368168
