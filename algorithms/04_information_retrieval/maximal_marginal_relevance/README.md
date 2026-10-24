# Maximal Marginal Relevance (MMR) Reranking

## Motivation

Retrieval systems can return several individually relevant but nearly
identical passages. **Maximal Marginal Relevance (MMR)** chooses candidates
that trade off their relevance to the query against similarity to
previously selected candidates. This is useful for diverse retrieval
and candidate selection in retrieval-augmented generation (RAG).
The technique was introduced by Carbonell and Goldstein (1998).

This is a deterministic **reranking primitive**. The caller supplies
relevance scores and pairwise similarities, not raw text or embeddings.

## Formula

Given n candidates, relevance scores `r[i]`, pairwise similarities
`S[i,j]`, diversity tradeoff `lambda ∈ [0,1]`, and a set of
previously selected indices `A`:

```text
MMR(i | A) =
    lambda * r[i]                               if A is empty
    lambda * r[i] - (1-lambda)*max(S[i,j] for j in A) otherwise

next = argmax_{i not in A} MMR(i | A)
```

Ties select the **smallest original zero-based index**. After a candidate
is selected, update each remaining candidate's maximum similarity to
any selected one. The first pick depends only on relevance (except
when lambda=0, when all first-step scores tie).

Worked example:

```text
relevance = [0.9,0.8,0.7]
similarity = [[1,0.9,-0.5],
              [0.9,1,0.0],
              [-0.5,0.0,1]]
lambda = 0.5; k=3
order = [0,2,1]
```

Candidate 2 is selected ahead of candidate 1 because it is less
similar to the already selected candidate 0.

## Shared input/output contract

- Input relevance is a finite numeric vector of length `n>=1`;
  all relevance scores must be in **[0,1]**.
- Similarity must be a finite real `n×n` matrix with all entries
  in **[-1,1]**. Its rows describe candidate-to-candidate similarity.
  Diagonal values need not be exactly one; the algorithm does not
  require symmetry, so non-symmetric similarity matrices are also
  accepted and interpreted directionally as `S[candidate,selected]`.
- `lambda` must be finite and in [0,1]. `k` must be an integer
  in [1,n]. All data is validated, including unselected candidates.
- Return **k distinct zero-based original indices in selection order**.
  There is no output score vector or model training.
- All languages apply the same tie-breaking convention. C/Rust
  accept flat **row-major** similarities; Python nested rows;
  R a numeric matrix. C requires caller-owned `size_t[k]` output,
  disjoint from input and unchanged on failure. Rust returns
  `Result<Vec<usize>,...>`; Python/R raise errors.
- Floating-point intermediate scores must be finite. R rejects
  non-integer k; Python rejects bools as numeric inputs.

## Complexity

Validating the full matrix costs **O(n²)** time. Greedy selection
and incremental maximum-similarity updates cost **O(k*n)** time.
Auxiliary memory is **O(n+k)** (plus O(n²) Python input copies).
No dynamic programming, embedding model or vector index is required.

## Implementation and tests

| Language | Directory |
| --- | --- |
| C | [maximal_marginal_relevance_c](maximal_marginal_relevance_c/) |
| Python | [maximal_marginal_relevance_python](maximal_marginal_relevance_python/) |
| Rust | [maximal_marginal_relevance_rust](maximal_marginal_relevance_rust/) |
| R | [maximal_marginal_relevance_r](maximal_marginal_relevance_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover novelty-driven selection, pure relevance (lambda=1),
pure diversity (lambda=0), exact ties, singleton inputs, invalid
matrices, negative or out-of-range scores, invalid cutoff and
nonfinite inputs.

## Limitations

MMR is **greedy**, and need not globally optimize every possible
diversity objective. Pairwise scores are assumed supplied and
meaningful; this code does not calculate embeddings, cosine similarity,
query scores or full document ranking. It does not guarantee a
submodularity bound for arbitrary matrices or combine results from
multiple models. No CUDA kernel or performance claim is made.

## Reference

Carbonell, J., & Goldstein, J. (1998). The use of MMR, diversity-based
reranking for reordering documents and producing summaries. In
*Proceedings of the 21st Annual International ACM SIGIR Conference on
Research and Development in Information Retrieval* (pp. 335–336).
https://doi.org/10.1145/290941.291025
