# Dijkstra Shortest Paths

## Why this algorithm belongs here

Shortest-path search underlies graph retrieval, network routing, and
several graph-based AI methods. **Dijkstra's algorithm** computes the
minimum accumulated cost from a chosen source to every reachable
vertex when all edge weights are nonnegative (Dijkstra, 1959).

This reference is deliberately dense, dependency-free and deterministic.

## Mathematical definition

An `n×n` adjacency matrix `W` encodes a **directed** graph.
`W[u,v]=0` means **no edge**; any strictly positive entry means
an edge with that cost. A diagonal zero is normal. This contract does
not support zero-cost edges.

Initialize `distance[source]=0`, all other distances to infinity.
Repeatedly choose the unvisited node `u` with smallest finite
distance (index ascending for ties) and relax its outgoing edges:

```text
distance[v] = min(distance[v], distance[u] + W[u,v])
                     when W[u,v] > 0 and v is unvisited
```

Stop when all remaining unvisited nodes are unreachable. Return
`-1` for unreachable nodes; all reachable distances are nonnegative.

Example:

```text
W = [[0,2,5,0],
     [0,0,1,0],
     [0,0,0,0],
     [0,0,0,0]]
source = 0
distances = [0,2,3,-1]
```

## Input/output contract

- `1 <= n <= 128`, source is an integer in `[0,n-1]`, weights
  are finite real numbers in **[0,1,000,000]**.
- Result is a length-n distance vector in original node order.
  `-1` means unreachable, including graph components without a
  path from the source. Self-loops and disconnected nodes are valid.
- Validate **all input weights**, including unreachable subgraphs.
  Reject malformed matrices, negative or nonfinite weights,
  invalid source, arithmetic overflow, or output allocation failure.
- C and Rust accept row-major `weights[n*n]`. C requires
  caller-owned `distances[n]` that **remain unchanged on failure**
  and do not overlap the input. Rust returns `Result<Vec<f64>,...>`.
  Python accepts nested lists/tuples; R accepts a numeric matrix.
- All implementations use strict improvement when relaxing a node
  and choose the lowest-index unvisited node for equal tentative
  distances. No predecessor path is returned.
- Nodes are **zero-based** in the API of all four languages, even R.

## Complexity

Dense linear-scan implementation: **O(n²)** time, **O(n)** additional
memory for distances and visited flags. This is not a heap-based
sparse-graph solver.

## Implementations and tests

| Language | Directory |
| --- | --- |
| C | [dijkstra_shortest_path_c](dijkstra_shortest_path_c/) |
| Python | [dijkstra_shortest_path_python](dijkstra_shortest_path_python/) |
| Rust | [dijkstra_shortest_path_rust](dijkstra_shortest_path_rust/) |
| R | [dijkstra_shortest_path_r](dijkstra_shortest_path_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover a shorter indirect route, unreachable vertices,
cycles, self-links, a singleton graph, tie distances, invalid input,
and negative or nonfinite edge weights.

## Limitations

No negative weights, explicit zero-weight edges, recovered routes,
dynamic graph updates, custom heuristics (A*), graph embeddings,
sparse priority queues, or device-tested GPU acceleration.

## Reference

Dijkstra, E. W. (1959). A note on two problems in connexion with graphs.
*Numerische Mathematik, 1*, 269–271.
https://doi.org/10.1007/BF01386390
