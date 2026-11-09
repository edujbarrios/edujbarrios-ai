# Edmonds–Karp Maximum Flow

## Graph and retrieval context

**Maximum flow** asks how much capacity can be routed from a source to a sink in a directed network while respecting edge capacities and flow conservation. It has applications to routing, assignment, matching and graph optimization (Cormen et al., 2022).

## Algorithm

Start with a **residual capacity matrix** equal to the directed capacity matrix. Repeatedly use **breadth-first search (BFS)** from source to sink along edges whose residual capacity is positive. If no such path exists, the accumulated flow is maximal. Otherwise determine the smallest residual capacity along the path (the bottleneck), subtract it from every forward residual edge, add it to each reverse residual edge, and add the bottleneck to the total. Repeat. BFS visits neighbors in ascending vertex order for deterministic behavior.

The reverse residual edges are essential: they permit later augmentations to cancel earlier allocations, even when the original graph already has directed capacity in both directions.

## Input/output contract

- `n` is an integer in **2..16**; `source` and `sink` are **distinct zero-based vertex indices** in `0..n-1`.
- Input is a **flat row-major n×n directed capacity matrix**, with integer capacities **0..1000**, and **zero diagonal** (no self-loop capacity). The matrix **need not be symmetric**. Zero indicates no capacity.
- Output: the maximum flow as a **nonnegative integer**. With these bounds its maximum cannot exceed 240,000, so a 32-bit integer is ample. The input matrix is never mutated.
- C: `bool edmonds_karp_max_flow(const uint16_t *capacities, size_t n, size_t source, size_t sink, uint32_t *flow_out)`. The output is untouched if validation fails.
- Python accepts a list or tuple of integer capacities, rejecting booleans and malformed types, and returns `int`. Rust returns `Result<u32,&str>`. R accepts one flat numeric integer-valued vector and returns one integer; rejects logical, complex, matrix, nonfinite, nonintegral or out-of-range values.
- If the sink cannot be reached, return `0`. Identical source and sink are **invalid**, rather than interpreted as zero or unlimited flow.

## Complexity

Edmonds–Karp performs **O(VE)** augmentations with BFS. With adjacency lists the total is O(VE²); this educational implementation scans an **n×n matrix** in each BFS, giving **O(V³E)** time (O(V⁵) on dense graphs), and O(V²) extra residual matrix storage plus O(V) queue/parent storage. Here V is at most 16.

## Examples

```text
2 vertices, 0→1 capacity 7: max_flow(0,1) = 7
3 vertices, 0→1 capacity 4, 1→2 capacity 2,
            0→2 capacity 1: max_flow(0,2) = 3
CLRS 6-vertex network (see tests): max_flow(0,5) = 23
No path from source to sink: max_flow = 0
```

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [edmonds_karp_max_flow_c](edmonds_karp_max_flow_c/) |
| Python | [edmonds_karp_max_flow_python](edmonds_karp_max_flow_python/) |
| Rust | [edmonds_karp_max_flow_rust](edmonds_karp_max_flow_rust/) |
| R | [edmonds_karp_max_flow_r](edmonds_karp_max_flow_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests cover the classic flow-23 graph, opposite-direction edges, disconnected networks, graph limits, invalid capacity values, self-loops and invalid source/sink.

## Limitations

Only directed integral capacities and a dense small graph are supported. This is not a minimum-cost flow solver, does not provide the flow on every original edge or a minimum cut, and does not accept floating-point capacities.

## References

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to algorithms* (4th ed.). MIT Press.
