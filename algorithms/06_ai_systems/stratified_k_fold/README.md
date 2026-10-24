# Deterministic Stratified K-Fold Assignment

## Why it belongs in AI

**K-fold cross-validation** evaluates models on repeated held-out
subsets. For classification, **stratification** distributes examples
from each class across folds so each validation subset retains
representation of both classes. This reduces the risk that a rare
class disappears from a test fold (Kohavi, 1995).

This reference implements the **fold assignment** step only, not
model training or resampling. It is deterministic and does not
shuffle examples automatically.

## Algorithm

Input labels `y[i] ∈ {0,1}` and an integer number of folds `k`.
Require **at least k samples in each class**.

Visit all class-0 examples in increasing original index order, then
all class-1 examples in increasing original index order. Keep a
single cumulative assignment counter `c`:

```text
c = 0
for class_label in [0,1]:
    for i where y[i] == class_label (in index order):
        fold[i] = c mod k
        c += 1
```

The counter **does not reset between classes**, so each class
occupies folds as evenly as possible and total fold sizes differ
by at most one. Every fold contains at least one example of each
class under the input constraints.

Example:

```text
labels = [0,1,0,1,0,1]
k = 3
folds = [0,0,1,1,2,2]

fold 0 validation indices = [0,1]
fold 1 validation indices = [2,3]
fold 2 validation indices = [4,5]
```

For any fold `f`, samples with `fold[i]==f` are validation
samples, and the remaining samples are training samples.
**Fit preprocessing (e.g., scaling, vocabulary selection, PCA)
only on the training subset for each fold** to prevent data leakage.

## Shared contract

- Input labels are a **nonempty binary integer sequence** of
  length `2..4096`, with values exactly 0 or 1.
- Integer `k` must satisfy `2<=k<=n`. **Each binary class must
  appear at least k times**; reject other inputs rather than
  creating validation folds that lack a class.
- Return n **zero-based fold IDs**, in original sample order,
  each in `[0,k-1]`. All k IDs occur, and total validation sizes
  differ by at most one. Each class is spread across folds with
  counts differing by at most one.
- C accepts `uint8_t labels[n]` and a caller-owned
  `uint32_t folds[n]`, with no overlap. Failed validation
  leaves output unchanged. Rust returns `Result<Vec<usize>,...>`,
  Python returns a list of integers, R an integer vector.
- Python rejects bool labels and noninteger k; R validates finite
  numeric integer-valued labels and k.

## Complexity

**O(n+k)** time and O(n) output memory, with O(1) auxiliary
storage. The algorithm does not copy features because it only
needs labels.

## Implementations

| Language | Directory |
| --- | --- |
| C | [stratified_k_fold_c](stratified_k_fold_c/) |
| Python | [stratified_k_fold_python](stratified_k_fold_python/) |
| Rust | [stratified_k_fold_rust](stratified_k_fold_rust/) |
| R | [stratified_k_fold_r](stratified_k_fold_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests check exact assignment fixtures, imbalanced yet feasible
class counts, fold-size differences, class representation, invalid
labels, too many folds and insufficient examples per class.

## Limitations

**No shuffle or random seed.** Ordered or time-correlated data
should not be assumed independent: if a shuffled split is desired,
randomize the row order *before* assigning folds, using an external,
explicitly controlled seed. Ordinary stratified K-fold is
inappropriate for temporal forecasting, grouped observations,
duplicate subjects spanning folds, or leakage-prone entities;
those require time-series or group-aware splitting. This is a
cross-validation primitive, not a guarantee of unbiased
generalization estimates. No GPU processing is necessary.

## References

Kohavi, R. (1995). A study of cross-validation and bootstrap for
accuracy estimation and model selection. In *Proceedings of the
14th International Joint Conference on Artificial Intelligence*
(pp. 1137–1143).
