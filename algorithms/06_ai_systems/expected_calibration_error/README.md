# Expected Calibration Error (ECE)

## Why it belongs in AI

A classifier can rank examples well yet assign confidence values that
do not correspond to its actual correctness rate. **Expected Calibration
Error (ECE)** is a diagnostic summary of the difference between
average predicted confidence and empirical accuracy in groups of
examples. It complements ROC AUC and the Brier score already included
in this catalogue (Guo et al., 2017).

ECE depends on its **binning and confidence conventions**. This
educational reference explicitly uses equal-width bins and the
confidence of the *predicted binary class*; it is **not** a statistically
unbiased calibration estimator or a class-one-probability residual.

## Mathematical definition

For class-one probability `p[i]∈[0,1]`, binary ground truth `y[i]`,
and integer bin count `B`:

```text
predicted[i] = 1 if p[i] >= 0.5 else 0    # tie -> class 1
confidence[i] = max(p[i], 1 - p[i])
correct[i] = 1 if predicted[i] == y[i] else 0

bin[i] = min(B - 1, floor(B * confidence[i]))  # zero-based

ECE = sum_{b with |I_b|>0} (|I_b|/n)
         * abs(mean(correct[i] in I_b) - mean(confidence[i] in I_b))
```

The bins partition **[0,1]** into equal widths: bin 0 covers
`[0,1/B)`, and the last bin includes the value 1.
For binary top-class confidence, the first half of the confidence
range is naturally empty. **Empty bins contribute 0**.

Example:

```text
probabilities = [0.9, 0.8, 0.2, 0.6]
labels        = [  1,   0,   0,   1]
confidence    = [0.9, 0.8, 0.8, 0.6]
correct       = [  1,   0,   1,   1]

with B=2: all examples occupy the last bin
mean confidence = 0.775, accuracy = 0.75
ECE = 0.025

with B=4: ECE = 0.225
```

## Shared contract

- Nonempty equal-length vectors of finite class-one probabilities
  in `[0,1]` and exact binary ground-truth labels 0 or 1.
- `bins` is an integer **in [1,100]**. Include a predicted
  probability of exactly 1 in the last bin.
- Return one finite scalar `ECE∈[0,1]`. The cutoff `p=0.5`
  predicts class 1; duplicate observations are counted separately.
- Validate all examples, including elements that might fall into an
  otherwise empty bin. Reject invalid probabilities, labels, shapes
  and bin counts. Python rejects booleans as numerical values; R
  requires finite numeric 0/1 labels and integral bin counts.
- C accepts `double probabilities[n]`, `uint8_t labels[n]`,
  `bins`, and a caller-owned `double* result` which must be
  disjoint from input and **unchanged on error**. Rust returns
  `Result<f64,...>`; Python/R raise errors.
- The statistic uses **unweighted examples** and predicted-class
  confidence, not per-class or adaptive-bin calibration.

## Complexity

**O(n+B)** time and **O(B)** auxiliary memory for n examples
and B bins. No model dependency, randomization or GPU code.

## Implementations

| Language | Directory |
| --- | --- |
| C | [expected_calibration_error_c](expected_calibration_error_c/) |
| Python | [expected_calibration_error_python](expected_calibration_error_python/) |
| Rust | [expected_calibration_error_rust](expected_calibration_error_rust/) |
| R | [expected_calibration_error_r](expected_calibration_error_r/) |

Run tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the 2- and 4-bin worked example, perfectly confident and
correct predictions, confidence ties, inclusive probability one,
single-bin behavior, invalid labels/probabilities and bin limits.

## Limitations

ECE is sensitive to sample size, choice of bins, and bin boundaries.
A small ECE does not prove a classifier is calibrated for every
subgroup. This implementation does not produce a reliability diagram,
confidence interval, classwise ECE, adaptive binning, temperature
scaling or probability-calibration model. No CUDA claims are made.

## References

Guo, C., Pleiss, G., Sun, Y., & Weinberger, K. Q. (2017). On
calibration of modern neural networks. In *Proceedings of the 34th
International Conference on Machine Learning* (Vol. 70,
pp. 1321–1330). PMLR.
https://proceedings.mlr.press/v70/guo17a.html
