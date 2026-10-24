# HITS Hub and Authority Scores

## Motivation

The **Hyperlink-Induced Topic Search (HITS)** algorithm assigns two
complementary scores to nodes in a directed graph: a good *authority*
is linked to by good *hubs*, while a good hub links to good
authorities. This is a classical link-analysis method useful for
understanding early search ranking and coupled graph importance
(Kleinberg, 1999).

Unlike PageRank, which produces one damped stationary distribution,
HITS produces **two mutually reinforcing vectors** without
teleportation.

## Mathematics

For nonnegative adjacency weights `W[i,j]` from node i to node j,
start with `h[i]=1/sqrt(n)` and repeat exactly **100** times:

```text
a_raw[j] = sum_i W[i,j] * h[i]  # authorities: W^T * hubs
a = a_raw / ||a_raw||_2

h_raw[i] = sum_j W[i,j] * a[j]  # hubs: W * authorities
h = h_raw / ||h_raw||_2
```

If either raw vector has zero L2 norm, define the corresponding
normalized vector as all zero. This also handles a graph without
links. **L2 normalization, not probability normalization**,
is the intended HITS convention. Scores are nonnegative and in
[0,1], but they are **not probabilities** and need not sum to 1.

Example, a two-leaf star:

```text
W = [[0,1,1],
     [0,0,0],
     [0,0,0]]

after 100 iterations:
authorities ≈ [0,1/sqrt(2),1/sqrt(2)]
hubs        ≈ [1,0,0]
```

For a directed cycle of equal positive weights, both vectors
remain uniform `1/sqrt(n)`.

## Shared contract

- `W` is a dense directed **n×n** matrix of finite real edge
  weights in **[0,1]**, with `1 <= n <= 64`. Zero means no link.
  Self-links are allowed; the graph need not be connected or
  symmetric.
- Run exactly **100 synchronous iterations**, in the same operation
  order in each language. No stopping tolerance, personalization,
  damping or graph sampling is used.
- Return **two arrays** of length n:
  `(authorities,hubs)` in node order. Either is the zero vector
  when the corresponding norm vanishes.
- Reject NaN/Inf, negative/excessive weights, incorrect dimensions
  or any nonfinite intermediate value.
- C/Rust consume flat row-major weights; Python consumes nested
  rows; R consumes a numeric matrix. C writes caller-owned
  `authorities[n]` and `hubs[n]` only on complete success.
  Both output arrays must be distinct and must not overlap inputs.
  Rust returns `Result<(Vec<f64>,Vec<f64>),...>`; Python a tuple
  of lists; R a named list.
- The initialization and 100-iteration cap are intentional.
  For graphs with degenerate top singular values, the scores
  can depend on initialization; no claim of unique HITS scores
  or rigorous convergence tolerance is made.

## Complexity

**O(100 n²)** time and **O(n)** extra storage beyond the
n×n input graph. For the fixed iteration count, time is O(n²).

## Implementations

| Language | Directory |
| --- | --- |
| C | [hits_hubs_authorities_c](hits_hubs_authorities_c/) |
| Python | [hits_hubs_authorities_python](hits_hubs_authorities_python/) |
| Rust | [hits_hubs_authorities_rust](hits_hubs_authorities_rust/) |
| R | [hits_hubs_authorities_r](hits_hubs_authorities_r/) |

Run the strict tests from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the star fixture, cycles, disconnected/edgeless
graphs, self-links and malformed adjacency weights.

## Limitations

Dense small-graph reference only. No query-conditioned subgraph
construction, iterative stopping criterion, sparse graph storage,
distributed iteration, graph neural network integration, or
device-tested CUDA kernels.

## References

Kleinberg, J. M. (1999). Authoritative sources in a hyperlinked
environment. *Journal of the ACM, 46*(5), 604–632.
https://doi.org/10.1145/324133.324140
