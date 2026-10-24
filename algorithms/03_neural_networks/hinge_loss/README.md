# Binary Hinge Loss and Subgradient

## Why it belongs in AI

Hinge loss underlies **maximum-margin classification** such as linear
Support Vector Machines (SVMs). Unlike cross-entropy, it directly
penalizes samples whose signed decision scores fail to achieve a unit
classification margin (Cortes & Vapnik, 1995). It also demonstrates
subgradients at a nondifferentiable boundary.

This primitive evaluates a batch of *already computed decision scores*,
and returns their mean loss and a subgradient with respect to each score.
It does not train an SVM or compute regularization terms.

## Mathematical definition

For `n` real scores `s[i]` and signed class labels `y[i] ∈ {-1,+1}`:

```text
margin[i] = y[i] * s[i]
loss[i] = max(0, 1 - margin[i])
L = sum(loss[i]) / n

dL / ds[i] = -y[i] / n  if margin[i] < 1
              0         if margin[i] >= 1
```

**At exactly margin=1**, use subgradient 0. This is a valid
subgradient of hinge loss at the kink and is deliberately defined for
deterministic agreement among implementations.

Example:

```text
scores = [2, 0, -0.5, 1]
labels = [1, 1, -1, -1]
loss per sample = [0, 1, 0.5, 2]
mean loss = 0.875
gradient = [0, -0.25, +0.25, +0.25]
```

## Shared contract

- Nonempty same-length vectors of finite real decision scores and
  **exact integer labels** `-1` or `1`.
- Return a finite nonnegative mean loss and a real gradient vector
  of length n, in the original sample order. No probabilities,
  thresholding or sample weights are applied.
- Reject NaN/Inf, malformed labels, length mismatches, or overflow in
  the multiplication, margin subtraction, loss accumulation, gradient
  division or final result. No clipping is performed.
- C accepts `double scores[n]`, `int8_t labels[n]`, and
  caller-owned `double* loss` plus `double gradients[n]`
  (all buffers nonoverlapping); outputs **stay unchanged on failure**.
  Rust returns `Result<(f64,Vec<f64>),...>`. Python returns
  `(loss,gradients)`, and R a named list.
- Python rejects bools; R accepts finite numeric labels exactly
  equal to ±1. C and Rust use signed 8-bit labels.
- The subgradient is with respect to model scores, **not parameters**.
  To get parameter gradients, apply the chain rule through the scoring
  model separately.

## Complexity

O(n) time, O(n) gradient output storage. C allocates scratch gradients
to guarantee no partial output writes.

## Implementations

| Language | Source |
| --- | --- |
| C | [hinge_loss_c](hinge_loss_c/) |
| Python | [hinge_loss_python](hinge_loss_python/) |
| Rust | [hinge_loss_rust](hinge_loss_rust/) |
| R | [hinge_loss_r](hinge_loss_r/) |

Run the tests from the root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests include active/inactive margins, exactly-one margin, both classes,
batch averaging, malformed vectors, nonfinite scores and overflow.

## Limitations

Binary signed-label hinge loss only. No multiclass hinge, squared
hinge, soft-margin regularization, SVM dual optimization, kernel trick,
automatic differentiation framework or GPU kernel. **The kink has
many valid subgradients**; this reference intentionally chooses zero.

## References

Cortes, C., & Vapnik, V. (1995). Support-vector networks.
*Machine Learning, 20*, 273–297.
https://doi.org/10.1007/BF00994018
