# Binary Decision Stump

## AI motivation

A **decision stump** is a one-level decision tree: it classifies data by
testing a single threshold. Stumps are interpretable weak learners often
used in ensemble methods such as boosting. Studying exact threshold
selection introduces greedy model fitting, classification error, and
tie-breaking without a full decision-tree library (Hastie et al., 2009).

## Model and algorithm

Given finite scalar features `x[i]` and binary labels `y[i] ∈ {0,1}`,
consider every **distinct observed feature value** as a threshold `t`.
Evaluate two orientations:

```text
positive_right = True:  prediction(x) = 1 if x > t else 0
positive_right = False: prediction(x) = 1 if x <= t else 0

training_errors(t, orientation)
  = sum(prediction(x[i]) != y[i])
```

Choose the candidate minimizing the **count of classification errors**.
When errors tie, choose the **numerically smallest threshold**; at the
same threshold choose `positive_right=True` before `False`. Candidate
thresholds are always actual observed input values, not midpoints.
This convention avoids midpoint overflow while remaining reproducible.

The two orientations let the algorithm represent both directions of
separation and constant-label datasets, including the case where all
features are identical.

## Worked example

```text
x      = [0, 1, 2, 3]
labels = [0, 0, 1, 1]

fitted threshold=1, positive_right=True, errors=0
predictions for [0, 1, 2, 3] -> [0, 0, 1, 1]
```

## Shared contract

- Accept two **nonempty**, equally sized arrays of finite real features
  and exact binary labels 0 or 1. No distinct-feature or class-balance
  requirement.
- Return `(threshold, positive_right, errors)`: finite feature
  threshold, a boolean orientation, and an integer training-error count.
  For R, orientation is a logical scalar and errors are an integer scalar.
  For C, orientation is a `uint8_t` (1 for right, 0 for left).
- A helper prediction function applies the fitted threshold to any
  additional **finite** scalar feature; it does not refit the stump.
- Python and R raise errors on invalid data; Rust returns `Result`.
  C returns `bool`, and must leave caller-owned outputs untouched on
  error. C requires allocated feature/label arrays of length `n`,
  and disjoint pointers for output threshold, orientation and errors.
- Candidates are enumerated in input order, but final selection obeys
  the documented **error, threshold, orientation** tie convention,
  so shuffling the sample rows does not change the fitted stump.
- Non-finite features, nonbinary labels, missing input, zero samples
  or invalid inference inputs are errors. Duplicate x values are valid.

## Complexity

A deliberately transparent exhaustive search considers up to `n`
thresholds, two orientations and `n` samples per candidate:
**O(n²)** time and **O(1)** auxiliary memory in C/Rust/R.
Python additionally copies and validates input arrays (O(n) memory).
A production stump can optimize fitting with sorted feature values
in `O(n log n)`, which is intentionally not implemented here.

## Implementations

| Language | Folder |
| --- | --- |
| C | [decision_stump_c](decision_stump_c/) |
| Python | [decision_stump_python](decision_stump_python/) |
| Rust | [decision_stump_rust](decision_stump_rust/) |
| R | [decision_stump_r](decision_stump_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict` from the repository root.
Tests include both orientations, shuffled data, ties, constant labels,
duplicate features, invalid labels and nonfinite input.

## Limitations

Single real feature, two classes, unweighted 0–1 loss, no pruning,
feature selection across multiple columns, probabilities, missing-value
handling, feature scaling, ensembles or continuous threshold refinement.
Training error is not a measure of generalization. No CUDA kernel or GPU
speed claims are included.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of
statistical learning: Data mining, inference, and prediction* (2nd ed.).
Springer. https://doi.org/10.1007/978-0-387-84858-7
