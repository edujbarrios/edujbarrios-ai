# A* Shortest Path with a Consistent Heuristic

## Why it belongs in AI

**A*** combines known cost from a starting node and an estimated
remaining cost to a target to direct graph search. It is a
foundational algorithm in AI planning, robotics, navigation, and
heuristic search (Hart et al., 1968). This reference complements
the repository's Dijkstra implementation by recovering an actual
**source-to-target path**, rather than only distance scores.

A* is guaranteed to return a shortest path in this implementation
because its supplied heuristic is **validated for consistency**.
An arbitrary heuristic is not silently assumed admissible.

## Mathematical definition

For a directed edge weight matrix `W[u,v]`, with `W[u,v]=0`
meaning **no edge** and positive weights indicating link costs,
assign each node `u` a finite nonnegative heuristic `h[u]`.
Require `h[target]=0` and, for every positive edge `u->v`:

```text
h[u] <= W[u,v] + h[v] + 1e-9
```

The small tolerance avoids rejecting heuristics because of tiny
floating-point rounding differences. A strictly consistent heuristic
with h[target]=0 is admissible along every path to the target.

Maintain `g[u]`, the best cost to each discovered node, and
`f[u] = g[u] + h[u]`. Repeatedly expand the unvisited discovered
node with the lowest `f` (ties by lowest node ID). Relax positive
outgoing edges using strict `g` improvement and store predecessors.
Stop at the target and reconstruct the path backward.

Example:

```text
W = [[0,2,5,0],
     [0,0,1,10],
     [0,0,0,2],
     [0,0,0,0]]

h = [4,3,2,0], source=0, target=3

path = [0,1,2,3]
distance = 5
```

If the target is unreachable, return `distance=-1` and an
**empty path**. A source equal to target returns distance zero
and a single-node path, after validating all input data.

## Shared contract

- Directed `n×n` nonnegative finite edge weights in
  **[0,1,000,000]**, for `1<=n<=64`; a zero weight means
  no edge (zero-cost edges are unsupported).
- Source and target are **zero-based integers in [0,n−1]**.
  Heuristic vector `h[n]` contains finite values in
  **[0,1,000,000]**, h[target] must be exactly 0, and every
  positive edge must satisfy the consistency condition above.
- Return **distance** and the sequence of zero-based node IDs
  forming the reconstructed shortest path. Tied expansions choose
  the smallest node ID, and only strict cost improvements update
  predecessors. No randomization.
- Reject malformed matrices, negative/nonfinite/excessive edges,
  incorrect node indices, inconsistent heuristics and nonfinite
  arithmetic. Validate the full graph, not just reachable nodes.
- C and Rust accept flat row-major adjacency weights. Python
  accepts nested rows; R accepts a base numeric matrix.
  C uses caller-owned `double* distance`, `size_t path[n]`,
  `size_t* path_length`, disjoint from all inputs. **No output
  changes on error**. Rust returns `Result<(f64,Vec<usize>),...>`,
  Python a tuple, and R a named list.
- This implementation uses a dense linear scan of open nodes.
  Consistency ensures nodes need not be reopened once closed.

## Complexity

**O(n²)** time for full edge/heuristic validation and dense A*
selection/relaxation, **O(n)** additional storage for best costs,
predecessors, visited flags, and the reconstructed path.

## Implementations

| Language | Directory |
| --- | --- |
| C | [astar_shortest_path_c](astar_shortest_path_c/) |
| Python | [astar_shortest_path_python](astar_shortest_path_python/) |
| Rust | [astar_shortest_path_rust](astar_shortest_path_rust/) |
| R | [astar_shortest_path_r](astar_shortest_path_r/) |

Run validation:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover indirect shortest paths, unreachable targets,
source==target, tied expansions, inconsistent heuristics, nonzero
goal heuristic, negative edges and invalid dimensions.

## Limitations

Small dense graph reference. No heuristic generation, admissibility
checking without consistency, heap priority queue, zero-cost edges,
negative edges, all-destination distances, dynamic edge updates,
geographic route metrics, or CUDA acceleration. For an unknown
consistent heuristic, all zeros is valid and A* reduces to Dijkstra
with the same expansion tie policy.

## References

Hart, P. E., Nilsson, N. J., & Raphael, B. (1968). A formal basis
for the heuristic determination of minimum cost paths.
*IEEE Transactions on Systems Science and Cybernetics, 4*(2),
100–107. https://doi.org/10.1109/TSSC.1968.300136
