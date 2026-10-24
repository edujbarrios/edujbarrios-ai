# Label-Smoothed Softmax Cross-Entropy and Gradient

## AI motivation

**Label smoothing** replaces a one-hot target with a mixture of
the hard label and a uniform categorical distribution. This is a
widely used regularization technique in neural-network training,
particularly sequence modeling, where one-hot confidence can lead
to overly sharp predictions (Szegedy et al., 2016).

This reference computes both the **loss and its analytical
gradient with respect to logits**, providing the objective needed
by a caller-managed training loop.

## Definition

For finite logits `z[0..n-1]`, true class `y`, and
smoothing coefficient `epsilon∈[0,1]`:

```text
p[i] = exp(z[i]-max(z)) / sum_j exp(z[j]-max(z))
q[i] = (1-epsilon)*[i == y] + epsilon/n

loss = -sum_i q[i]*ln(p[i])
gradient[i] = p[i] - q[i]
```

The implementation computes loss from stable **log probabilities**,
not `ln(p[i])` on possibly underflowed softmax outputs:

```text
shifted[i] = z[i] - max(z)
logZ = ln(sum_j exp(shifted[j]))
loss = sum_i q[i]*(logZ - shifted[i])
```

The formula deliberately smooths over **all n classes including
the true class**, not only the other `n−1` classes. The model
may have just one class, giving zero loss and zero gradient.

Worked example:

```text
logits = [0, ln(3)]  ->  p = [0.25, 0.75]
true class = 1
epsilon = 0.2
q = [0.1, 0.9]
loss = -0.1*ln(0.25) - 0.9*ln(0.75) ≈ 0.3973
gradient = [0.15,-0.15]
```

## Shared contract

- `1<=n<=4096` finite real logits; exact integer target
  index `0<=y<n`; finite smoothing `0<=epsilon<=1`.
- Return one finite nonnegative cross-entropy scalar and an aligned
  gradient vector of n finite real values. Inputs are never mutated.
- Softmax may underflow an extremely small class weight to zero;
  the log-space loss remains meaningful. However, reject
  nonrepresentable shifted logits, exponentials, sums, loss terms
  and gradients. Inputs such as `[-1e308,1e308]` violate
  the finite-difference arithmetic contract.
- C uses `double logits[n]`, caller-owned `double* loss`
  and `double gradient[n]`. Both outputs are **unchanged
  on error**, and buffers must be disjoint and sized correctly.
  Rust returns `Result<(f64,Vec<f64>),...>`;
  Python returns `(loss,list)`; R returns a named list.
- Python rejects booleans as logits, class indices, or smoothing
  values. R requires an integer-valued target index and numeric
  logits/smoothing. All APIs use **zero-based class indices**.
- These are *single-sample* loss/gradient outputs; batch reduction,
  logits from a model, optimizer updates and masking are left to
  the caller.

## Complexity

**O(n)** time and O(n) memory. The C implementation allocates
a temporary gradient vector and writes output only on success.

## Implementations

| Language | Directory |
| --- | --- |
| C | [label_smoothed_cross_entropy_c](label_smoothed_cross_entropy_c/) |
| Python | [label_smoothed_cross_entropy_python](label_smoothed_cross_entropy_python/) |
| Rust | [label_smoothed_cross_entropy_rust](label_smoothed_cross_entropy_rust/) |
| R | [label_smoothed_cross_entropy_r](label_smoothed_cross_entropy_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the numerical fixture, epsilon 0 and 1, equal
logits, singleton classes, large common offsets, invalid indices,
smoothing, NaN, infinity and unrepresentable logit differences.

## Limitations

No minibatching, class weights, padding mask, sampled softmax,
model training or fused GPU kernel. Label smoothing need not
improve every task or every calibration metric; it is an optional
regularizer, not a universal correction for overconfidence.

## Reference

Szegedy, C., Vanhoucke, V., Ioffe, S., Shlens, J., & Wojna, Z.
(2016). Rethinking the Inception architecture for computer vision.
In *Proceedings of the IEEE Conference on Computer Vision and
Pattern Recognition* (pp. 2818–2826).
https://doi.org/10.1109/CVPR.2016.308
