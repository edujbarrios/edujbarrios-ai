# Binary Precision–Recall Curve

## Why it belongs in AI

Precision and recall expose the trade-off between retrieved or predicted positives
and missed relevant items. A threshold sweep is especially informative for
imbalanced binary classification and retrieval evaluation (Manning et al., 2008).

## Algorithm

For each distinct score threshold t, classify every observation with score >= t
as positive, and report:

- precision(t) = TP(t) / (TP(t) + FP(t))
- recall(t) = TP(t) / P, where P is the total number of true positives

Sort scores from highest to lowest; **process all observations with the same
score together** before emitting a curve point. An observation contributes
to the threshold where its score first becomes included. No artificial
(precision=1, recall=0) endpoint is added. This is a threshold curve, not
interpolated average precision or an area-under-curve value.

## Contract

- Inputs: two aligned, nonempty vectors, scores of finite real probabilities
  in [0,1] and labels exactly 0 or 1. At least one positive label is required;
  all-positive datasets are permitted.
- Output: points (threshold, precision, recall) in decreasing threshold
  order. Each unique score yields one point; exact ties are grouped.
- Empty or mismatched vectors, NaN, Inf, out-of-range scores, nonbinary labels,
  and datasets with zero positives are rejected.
- All calculations use double precision and exact floating-point score equality
  for ties, without a near-tie tolerance. C requires independent, caller-owned
  output capacity for n points and leaves it untouched on failure.
- Python returns tuples, Rust returns tuples, C writes structures, and R
  returns a three-column numeric matrix. No random seed is needed.

Example: scores [0.8, 0.8, 0.5, 0.2], labels [1, 0, 1, 0].

| Threshold | Precision | Recall |
| --- | --- | --- |
| 0.8 | 1/2 | 1/2 |
| 0.5 | 2/3 | 1 |
| 0.2 | 1/2 | 1 |

## Complexity

Sorting dominates: O(n log n) time and O(n) additional memory. The subsequent
grouped sweep is O(n); there are at most n output points.

## Implementations

| Language | Directory |
| --- | --- |
| C | [precision_recall_curve_c](precision_recall_curve_c/) |
| Python | [precision_recall_curve_python](precision_recall_curve_python/) |
| Rust | [precision_recall_curve_rust](precision_recall_curve_rust/) |
| R | [precision_recall_curve_r](precision_recall_curve_r/) |

Run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This exact in-memory sweep is unsuitable for unbounded score streams without
external sorting. Thresholds are individual observed scores; there is no
interpolation, weighted sample handling, confidence interval, or numerical
integration. Floating-point scores that differ by even one representable
increment are distinct thresholds.

## References

Manning, C. D., Raghavan, P., & Schütze, H. (2008). *Introduction to information
retrieval*. Cambridge University Press. https://nlp.stanford.edu/IR-book/
