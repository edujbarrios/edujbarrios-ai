# Normalized Discounted Cumulative Gain at K (NDCG@K)

## Why it belongs in the AI hub

Search engines, recommender systems and retrieval-augmented generation need
metrics that reward placing **highly relevant** results early. Unlike binary
Precision@K and Recall@K, NDCG supports graded relevance judgments.
Järvelin and Kekäläinen (2002) introduced cumulative-gain evaluation for
ranking; this implementation uses the common exponential-gain convention.

## Mathematical definition

For graded relevance `r_i` at zero-based index `i` and cutoff `k`:

```text
gain(r)      = 2^r - 1
DCG@k        = sum(gain(r_i) / log2(i + 2), i = 0 .. k-1)
IDCG@k       = DCG@k of the ideal descending ordering of all supplied grades
NDCG@k       = DCG@k / IDCG@k   (or 0 if IDCG@k == 0)
```

Example: grades `[3, 2, 0, 1]`, `k=3`. Ideal grades are
`[3, 2, 1, 0]`, and the returned NDCG is approximately `0.9468`.
A perfectly graded order yields 1; entirely zero grades yield 0.

## Shared contract

- Input: a **nonempty** ranking of integer grades `0..20` inclusive and an
  integer cutoff `1 <= k <= n`. Grades are validated **including after k**.
- Output: a finite floating-point value in `[0,1]`. The value is clamped
  to that range to absorb small rounding excursions.
- The ideal ranking is formed by sorting the grades in descending order.
  This treats the provided list as the set of judged candidates;
  **it does not infer unseen relevant documents from the corpus**.
- Python and R reject invalid grades/k with errors. Rust returns
  `Result<f64, ...>`, and C returns `bool`, writing the output only after
  validation and successful computation.
- C accepts a `uint8_t` input and requires at least `n` elements; grades
  above 20 are invalid. Rust also accepts `u8`; Python rejects bool grades.
  R accepts finite integer-valued numeric grades in the common range.
- No randomization, external ML frameworks or non-reproducible tie-breaking.
  Different documents with the same grade contribute identically to IDCG.

## Complexity

**O(n log n)** time for the ideal sort plus O(k) scoring, and **O(n)**
auxiliary space to copy the grades for sorting. The method is dense and
intended as an explanatory reference, not a high-performance evaluator.

## Implementations

| Language | Folder |
| --- | --- |
| C | [ndcg_at_k_c](ndcg_at_k_c/) |
| Python | [ndcg_at_k_python](ndcg_at_k_python/) |
| Rust | [ndcg_at_k_rust](ndcg_at_k_rust/) |
| R | [ndcg_at_k_r](ndcg_at_k_r/) |

## Testing

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from the repository root.
Tests cover ideal ranking, non-ideal order, all-zero labels, invalid
grade after k, empty inputs and invalid cutoffs. CUDA is not part of this
single-query reference implementation.

## Limitations

This uses exponential gain `2^r-1`; some systems use linear gain instead.
Results are **not comparable** without reporting the gain convention, cutoff
and judgment pool. This code does not compute corpus-level averages,
confidence intervals, inferred NDCG or document deduplication.

## References

Järvelin, K., & Kekäläinen, J. (2002). Cumulated gain-based evaluation of IR
techniques. *ACM Transactions on Information Systems, 20*(4), 422–446.
https://doi.org/10.1145/582415.582418
