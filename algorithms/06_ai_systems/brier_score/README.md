# Binary Brier Score

## Why it belongs in AI

A classifier can have a high ROC AUC while producing poorly calibrated
probabilities. The **Brier score** evaluates the quality of predicted
probabilities by comparing each probability of class 1 against the
observed binary label. It penalizes both overconfident errors and
inaccurate probabilities. It is a **strictly proper scoring rule**
for probabilistic binary forecasts (Brier, 1950).

This algorithm complements the repository's ROC AUC, F1 and confusion
matrix metrics. Unlike thresholded accuracy, it does not convert
probabilities into hard class decisions.

## Formula

Given `n>=1` predicted class-one probabilities `p[i] ∈ [0,1]`
and binary labels `y[i] ∈ {0,1}`:

```text
Brier = (1/n) * sum_i (p[i] - y[i])^2
```

Lower is better: **0** is perfect, **1** is maximally wrong when every
prediction assigns probability 1 to the incorrect outcome.
An uninformative constant forecast of 0.5 always scores 0.25.

Example:

```text
probabilities = [0.1, 0.8, 0.6, 0.2]
labels        = [0,   1,   1,   0]

squared errors = [0.01,0.04,0.16,0.04]
Brier = 0.0625
```

## Shared contract

- Probability vector and label vector must be **nonempty and of the
  same length**.
- Probabilities must be finite real numbers in **[0,1]**, inclusive;
  labels must be exact binary integers 0 or 1.
- Return a finite scalar in **[0,1]**. No threshold, normalization
  by class prevalence, or clipping of invalid probabilities is applied.
- All elements must be valid, even if an early observation already
  indicates an error. Python rejects bools as numeric inputs/labels;
  R requires finite numeric labels equal to 0 or 1.
- C uses caller-provided `double probabilities[n]`,
  `uint8_t labels[n]`, and a single output `double*`; the
  output remains unmodified if any input is invalid or calculation
  cannot be represented. C inputs and output must not alias.
  Rust returns `Result<f64,...>`; Python/R raise errors.
- Calculation accumulates each squared error in a double and
  divides once by n. Floating-point comparisons in tests use
  tolerances rather than assuming exact representation of decimals.

## Complexity

**O(n)** time and **O(1)** extra space in all four languages.

## Implementations

| Language | Directory |
| --- | --- |
| C | [brier_score_c](brier_score_c/) |
| Python | [brier_score_python](brier_score_python/) |
| Rust | [brier_score_rust](brier_score_rust/) |
| R | [brier_score_r](brier_score_r/) |

Run repository tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the numerical example, perfect forecasts, confident
mistakes, constant 0.5 probabilities, a one-observation case,
invalid probabilities/labels, missing data and input-size mismatches.

## Limitations

Binary classification only; multiclass Brier uses a different
normalization and sum over classes. This score does not report
reliability diagrams, calibration curves, discrimination decomposition,
uncertainty intervals or calibration by groups. The Brier score alone
cannot explain how a probability model will behave under changing
base rates or distribution shift. No GPU benchmark or CUDA claim.

## Reference

Brier, G. W. (1950). Verification of forecasts expressed in terms
of probability. *Monthly Weather Review, 78*(1), 1–3.
https://doi.org/10.1175/1520-0493(1950)078%3C0001:VOFEIT%3E2.0.CO;2
