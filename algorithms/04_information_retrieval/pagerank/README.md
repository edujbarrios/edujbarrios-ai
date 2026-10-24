# PageRank by Power Iteration

## Why it belongs in AI

**PageRank** estimates the importance of pages or graph nodes using
the link structure of a directed graph. It is a foundational
information-retrieval and graph-ranking algorithm. A damping factor
combines link-following behavior with random teleportation, and
*dangling* nodes with no outgoing edges must be handled explicitly
(Brin & Page, 1998).

This implementation illustrates **fixed-iteration power iteration**
over a small dense weighted graph. It is not a web crawler, sparse
graph engine, or convergence-certified solver.

## Mathematics

Let `W[i,j]>=0` be the **weight of a directed link from node i to
node j**, and `out[i] = sum_j W[i,j]`. Row i of the link transition
matrix is:

```text
P[i,j] = W[i,j] / out[i]  if out[i] > 0
P[i,j] = 1 / n           if out[i] = 0 (dangling row)
```

Starting with the uniform vector `r[0,j]=1/n`, repeat exactly
**100 synchronous iterations** with fixed damping `d=0.85`:

```text
r[t+1,j] = (1-d)/n + d * sum_i r[t,i] * P[i,j]
```

The output is the final rank vector in **original node order**.
No sorting is performed. Teleportation and dangling redistribution
ensure every node gets some probability mass.

Example:

```text
W = [[0,1],
     [0,0]]

node 0 links to node 1; node 1 is dangling.
PageRank after 100 iterations ≈ [1/2.85, 1 - 1/2.85]
                              ≈ [0.35087719, 0.64912281]
```

For an all-dangling graph, or a symmetric directed cycle, scores
remain uniform.

## Shared contract

- Input: a nonempty **n×n** matrix of nonnegative finite edge
  weights in **[0,1,000,000]**, with **1<=n<=64**.
  Self-links are permitted, and edges with weight 0 are absent.
- Output: n finite nonnegative scores, in node order, summing to
  approximately one (floating-point rounding allowed). The
  damping factor is fixed at **0.85** and the number of
  synchronous iterations is fixed at **100** for deterministic
  behavior across languages.
- Reject malformed, negative, nonfinite or excessive edge weights,
  and any nonfinite row sum, update term, or rank.
- C and Rust read **flat row-major** weights; Python accepts nested
  rows; R a numeric matrix. C writes caller-owned `scores[n]`
  only on complete success, and input/output buffers cannot overlap.
  Rust returns `Result<Vec<f64>,...>`; Python/R raise errors.
  Python rejects booleans and R rejects invalid matrix types.
- **100 iterations is a fixed numerical approximation**. The code
  does not test residual convergence or guarantee any particular
  absolute error for arbitrary graphs. The positive teleport
  distribution makes the target stationary distribution unique.

## Complexity

**O(100 n²)** time and **O(n)** additional memory, beyond the
n×n input matrix. In this fixed-reference implementation this is
O(n²) time with a constant number of iterations.

## Implementations

| Language | Directory |
| --- | --- |
| C | [pagerank_c](pagerank_c/) |
| Python | [pagerank_python](pagerank_python/) |
| Rust | [pagerank_rust](pagerank_rust/) |
| R | [pagerank_r](pagerank_r/) |

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include a dangling-node analytic fixture, symmetric cycles,
all-dangling graphs, a singleton, weighted transitions, invalid
shapes, negative edges, and nonfinite values.

## Limitations

Dense small-graph educational reference only. It does not read
webpages, fetch links, rank raw text, implement personalization,
streaming graph changes, asynchronous sparse multiplication,
distributed PageRank, or acceleration on a tested GPU.
The rank is only meaningful under an appropriate graph model.

## References

Brin, S., & Page, L. (1998). The anatomy of a large-scale hypertextual
Web search engine. *Computer Networks and ISDN Systems, 30*(1–7),
107–117. https://doi.org/10.1016/S0169-7552(98)00110-X
