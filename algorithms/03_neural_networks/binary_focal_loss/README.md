# Binary Focal Loss

## Why it belongs in AI

**Focal loss** downweights easy, confidently classified examples and
focuses training on more difficult cases. It was introduced to address
severe foreground/background imbalance in dense object detection and
is a useful extension to the catalogue's cross-entropy and binary
cross-entropy functions (Lin et al., 2017).

This educational reference calculates a batch **mean focal loss**
over class-one probabilities; it does not return gradients, perform
automatic differentiation or train a model.

## Mathematical definition

Given class-one probability `p[i]` and binary label `y[i]`:

```text
p_t     = p[i]       when y[i] = 1
        = 1-p[i]     when y[i] = 0

alpha_t = 0.25       when y[i] = 1
        = 0.75       when y[i] = 0

q       = max(p_t, 1e-12)

focal_i = -alpha_t * (1-q)^2 * ln(q)
L       = mean_i focal_i
```

This reference fixes **alpha=0.25**, **gamma=2** and logarithm-floor
**epsilon=1e-12**. Using `q` for both the modulating term and the
logarithm makes the behavior at probability zero well defined.

For a true-class probability of 1, focal loss is exactly 0. For a
true-class probability of 0, it is a finite, large clipped penalty;
no attempt is made to return infinity.

Example:

```text
probabilities = [0.5,0.5]
labels        = [1,0]

individual losses = [ln(2)/16, 3*ln(2)/16]
mean focal loss = ln(2)/8 ≈ 0.08664339757
```

For `[0,1]` probabilities and matching `[0,1]` labels, the loss
is zero. For confident mistakes, the loss is substantially larger.

## Shared contract

- Accept nonempty, equal-length probability and label vectors.
- Probabilities must be finite real numbers **in [0,1]**, including
  endpoints; labels must be exactly binary integers 0 or 1.
- Return a finite nonnegative scalar batch-mean focal loss.
  All examples are equally weighted in the batch mean; the
  positive/negative alpha weights are applied inside the formula.
- Reject out-of-range probabilities, NaN, infinity, bad labels,
  malformed lengths and nonfinite arithmetic intermediates.
- C uses `double probabilities[n]`, `uint8_t labels[n]`,
  and caller-owned `double* result` with disjoint buffers.
  Failure leaves the output **unchanged**.
  Rust returns `Result<f64,...>`; Python/R raise errors.
  Python explicitly rejects boolean probabilities or labels.
- The function receives **probabilities, not logits**.
  Do not pass unnormalized scores without a sigmoid or other
  appropriate probability conversion.

## Complexity

**O(n)** time and O(1) auxiliary memory in all four languages.
No external ML framework or dependency is needed.

## Implementations

| Language | Directory |
| --- | --- |
| C | [binary_focal_loss_c](binary_focal_loss_c/) |
| Python | [binary_focal_loss_python](binary_focal_loss_python/) |
| Rust | [binary_focal_loss_rust](binary_focal_loss_rust/) |
| R | [binary_focal_loss_r](binary_focal_loss_r/) |

Run tests:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the analytic half-probability example, perfect
predictions, confident mistakes, zero/one endpoints, invalid
probabilities/labels and shape mismatches.

## Limitations

Binary only; fixed alpha, gamma and epsilon are educational choices,
not universally optimal hyperparameters. No generalized multiclass
focal loss, per-observation weights, model gradients, logit-space
stable formulation, training loop, label smoothing or CUDA kernels.
For extreme logits, a specialized logit-space implementation can
offer better numerical properties than precomputed probabilities.

## References

Lin, T.-Y., Goyal, P., Girshick, R., He, K., & Dollár, P. (2017).
Focal loss for dense object detection. In *Proceedings of the IEEE
International Conference on Computer Vision* (pp. 2980–2988).
https://doi.org/10.1109/ICCV.2017.324
