# Rocchio Relevance Feedback for Vector Queries

## Why it belongs in AI

**Rocchio relevance feedback** reformulates a search query using
document examples judged relevant and nonrelevant. It moves the query
toward the centroid of relevant documents and away from the centroid
of nonrelevant documents, making vector-space retrieval more
responsive to user feedback (Manning et al., 2008).

This reference implements **one deterministic query update**, not
a document retrieval loop or a learned ranking model.

## Formula

Given nonnegative query `q∈R^d`, relevant document vectors
`R={r_i}`, nonrelevant vectors `N={n_i}`, and fixed coefficients

```text
alpha=1.0, beta=0.75, gamma=0.15

q_new[j] = max(0, alpha*q[j]
                 + beta*mean_i(r_i[j])
                 - gamma*mean_i(n_i[j]))
```

An absent relevant or nonrelevant group contributes **zero**, not
an undefined mean. After the linear update, clip negative coordinates
to zero to maintain nonnegative vector-space weights.

Example:

```text
q = [1,1]
relevant    = [[2,0],[4,2]]
nonrelevant = [[0,2],[2,2]]

mean(relevant) = [3,1]
mean(nonrelevant) = [1,2]
q_new = [3.1,1.45]
```

The query may become all zeros after strong negative feedback.
The result is not normalized: a search system can apply its chosen
normalization/ranking scheme afterward.

## Shared contract

- Query has **1..64** finite nonnegative coordinates in **[0,1e6]**.
- Relevant and nonrelevant document groups each contain
  **0..128** rows of exactly the same feature width. Every document
  coordinate must be finite and in **[0,1e6]**. **At least one**
  of the two groups must be nonempty.
- Return a nonnegative finite query vector of length d. The original
  query and document vectors are unchanged.
- Validate all input groups, including the group that might be empty
  and rows that do not meaningfully affect the clipped result.
  Reject nonrectangular arrays, mismatched columns, negative
  features, invalid numerics and nonfinite arithmetic.
- C receives `query[d]`, optional flat row-major arrays
  `relevant[nr*d]`, `nonrelevant[ni*d]` (NULL accepted only
  when the respective row count is zero), and a caller-owned
  **non-overlapping** `result[d]` that is **unchanged on error**.
  Rust likewise uses flat row-major document matrices and
  receives both row counts. Python uses nested row sequences.
  R accepts two base numeric matrices (including 0-row,
  d-column matrices).
- No TF-IDF weights or embeddings are computed by the function;
  inputs must already share the **same vector dimensions**.

## Complexity

**O((nr+ni)*d)** time and **O(d)** auxiliary storage.
C allocates a temporary output array to avoid partial writes.

## Implementations

| Language | Directory |
| --- | --- |
| C | [rocchio_relevance_feedback_c](rocchio_relevance_feedback_c/) |
| Python | [rocchio_relevance_feedback_python](rocchio_relevance_feedback_python/) |
| Rust | [rocchio_relevance_feedback_rust](rocchio_relevance_feedback_rust/) |
| R | [rocchio_relevance_feedback_r](rocchio_relevance_feedback_r/) |

Run the full CPU test suites:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover two-sided feedback, positive-only and negative-only
feedback, zero clipping, mismatched dimensions, empty feedback,
nonfinite entries and out-of-range data.

## Limitations

One-step feedback only, with **fixed heuristic coefficients**.
No interactive judgments, automatic pseudo-relevance feedback,
document vector creation, TF-IDF fitting, iterative optimization,
retrieval quality measurement or GPU acceleration. Practical tuning
should be evaluated on held-out retrieval queries.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008).
*Introduction to information retrieval*. Cambridge University Press.
https://nlp.stanford.edu/IR-book/
