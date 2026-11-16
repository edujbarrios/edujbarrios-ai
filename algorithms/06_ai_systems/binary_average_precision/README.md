# Binary Classification Average Precision

## Why it belongs in AI

When the positive class is rare, accuracy and ROC-AUC alone can hide
poor positive-class precision. **Average precision (AP)** summarizes
the precision–recall trade-off for a ranked binary classifier.
Precision–recall analysis is especially informative in imbalanced
classification (Davis & Goadrich, 2006).

This differs from the repository's `average_precision_at_k` entry,
which evaluates retrieval relevance at a fixed ranking cutoff.
Here, predictions are continuous classification scores and all
distinct score thresholds are considered.

## Algorithm

Sort observations by **descending score**, grouping equal scores so
results do not depend on the input order within a tie. For each
group, update cumulative true positives `TP` and false positives
`FP`, and calculate:

```text
precision = TP / (TP + FP)
recall = TP / total_positives
AP += (recall - previous_recall) * precision
previous_recall = recall
```

This is **non-interpolated** AP: recall-weighted precision at
distinct thresholds. It is not the trapezoidal area under a
piecewise-linear precision–recall curve.

## Contract

- `binary_average_precision(y_true, y_scores)` accepts aligned
  list/tuple inputs of length **1..10000**.
- Labels are exactly integer `0` or `1` (not booleans).
  Scores are finite, non-boolean real numbers bounded by
  `[-1_000_000, 1_000_000]`. Larger scores indicate more
  confidence in the positive class.
- Return one float in `[0,1]`. When there are **no positive
  labels**, return `0.0` by explicit convention; with all
  positive labels AP is `1.0`.
- Tied scores are evaluated as one threshold. Inputs are not
  modified. Invalid arrays, scores, or labels raise `ValueError`.
- Binary64 arithmetic is used. Tiny numerical overshoots
  beyond one due to rounding are clamped to `1.0`.

## Worked example

```python
binary_average_precision([1,0,1,0], [0.9,0.8,0.7,0.1])
# 5 / 6 = 0.8333333333333333
```

The positives enter at ranks 1 and 3. Their precisions are
`1/1` and `2/3`; their mean is `5/6`.

With identical scores for all examples, AP equals the positive
class prevalence, independent of input order.

## Complexity

Sorting `n` samples takes **O(n log n)** time. Threshold grouping
takes O(n) time, with **O(n)** auxiliary storage for the sorted
score–label pairs.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [binary_average_precision_python](binary_average_precision_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover exact ranked examples, score ties and permutations,
single-class labels, extreme finite scores, input validation,
and immutability.

## Limitations

AP describes ranking quality but not calibration or the optimal
operating threshold. It can vary strongly with positive prevalence,
so comparisons across datasets should be interpreted carefully.
No sample weights, class-specific costs, confidence intervals,
or multiclass averaging are included.

## References

Davis, J., & Goadrich, M. (2006). The relationship between
Precision-Recall and ROC curves. In *Proceedings of the 23rd
International Conference on Machine Learning* (pp. 233–240).
https://doi.org/10.1145/1143844.1143874
