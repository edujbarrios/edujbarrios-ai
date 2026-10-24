# Binary ROC AUC

## Why it belongs in AI

The **area under the receiver operating characteristic curve** (ROC AUC)
measures how well a scoring classifier ranks positive examples above
negative examples across possible decision thresholds. It is widely used
alongside confusion matrices and F1 to evaluate classification systems.
AUC has a pairwise-ranking interpretation: it is the probability a randomly
selected positive receives a higher score than a randomly selected
negative, counting tied scores as half credit (Fawcett, 2006).

## Mathematics and tie handling

Given scores `s_i` and binary ground-truth labels `y_i ∈ {0,1}`,
sort scores in **ascending order**. Assign each tied-score group the
average of the 1-based ranks it occupies. Let `R_pos` be the sum of
ranks belonging to positive examples, `P` the number of positives and
`N` the number of negatives:

```text
U = R_pos - P*(P+1)/2
AUC = U / (P*N)
```

The average-rank rule awards a tied positive/negative pair half credit.

Example:

```text
scores = [0.1, 0.4, 0.35, 0.8]
labels = [  0,   0,    1,   1]
AUC = 0.75
```

Two identical scores with opposite labels yield AUC = 0.5.
Perfect ranking returns 1; completely reversed ranking returns 0.

## Shared contract

- Accept **equal-length, nonempty** score and label vectors. Scores
  must be finite real numbers (they **need not be probabilities**);
  labels must be exact binary integers 0 or 1.
- Require **at least one positive and at least one negative** sample.
  AUC is undefined when only one class is present and must be rejected,
  rather than silently set to zero.
- Return a finite scalar in `[0,1]`. Handle scores that are exactly
  tied by average rank, including signed-zero ties. Do not round scores
  or introduce a numerical tie tolerance.
- Validate all input labels and scores before sorting. Python rejects
  booleans as numbers or labels; R requires finite numeric labels equal
  to 0 or 1. C/Rust use `uint8_t` or `u8` labels.
- C takes `double scores[n]`, `uint8_t labels[n]` and a caller-owned
  scalar output; leaves the output untouched on errors. Rust returns
  `Result<f64,...>`; Python/R raise errors.
- Scores are ordered by their numeric value. The original sample order
  does not affect the metric; no external ML library is required.

## Complexity

Sorting `n` score/label pairs takes **O(n log n)** time and O(n) memory.
Grouping ties and calculating rank sums takes O(n) additional time.

## Implementations

| Language | Folder |
| --- | --- |
| C | [roc_auc_c](roc_auc_c/) |
| Python | [roc_auc_python](roc_auc_python/) |
| Rust | [roc_auc_rust](roc_auc_rust/) |
| R | [roc_auc_r](roc_auc_r/) |

## Tests and examples

```text
roc_auc([0.1, 0.4, 0.35, 0.8], [0,0,1,1]) = 0.75
roc_auc([0.5, 0.5], [0,1]) = 0.5
roc_auc([0.9, 0.1], [0,1]) = 0
roc_auc([0.1, 0.9], [0,1]) = 1
```

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

## Limitations

Binary labels only. No multiclass or multilabel aggregation,
calibration, bootstrapped confidence intervals, sample weights, partial
AUC, or precision–recall AUC. ROC AUC can be misleading when applied
without examining prevalence, operating thresholds, and error costs.
This reference uses standard floating-point ranks and does not claim
production robustness for datasets larger than practical memory limits.

## References

Fawcett, T. (2006). An introduction to ROC analysis. *Pattern Recognition
Letters, 27*(8), 861–874. https://doi.org/10.1016/j.patrec.2005.10.010
