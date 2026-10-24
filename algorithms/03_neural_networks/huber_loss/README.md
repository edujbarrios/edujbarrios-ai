# Huber Regression Loss and Gradient

## Why it belongs in AI

The **Huber loss** combines squared-error behavior for small
prediction errors with absolute-error-like behavior for large errors.
This makes it useful for robust regression and for training models
that may encounter noisy labels or outliers. Unlike pure squared
error, the magnitude of its gradient stops increasing once errors
exceed the transition threshold (Huber, 1964).

This standalone reference returns the **mean loss and gradient with
respect to each prediction**. It does not implement model
training or an optimizer.

## Formula

For predicted values `yhat[i]`, targets `y[i]`, residuals
`e[i]=yhat[i]-y[i]`, and a strictly positive threshold `delta`:

```text
huber(e) = 0.5 * e^2                    if |e| <= delta
           delta * (|e| - 0.5*delta)  if |e| > delta

loss = sum_i huber(e[i]) / n

d loss / d yhat[i] = e[i]/n                if |e[i]| <= delta
                     delta * sign(e[i])/n otherwise
```

The derivative is continuous at `|e|=delta` because both branches
return `+delta` or `-delta` there. There is no arbitrary
subgradient convention.

Example:

```text
predictions = [2,0,-2]
targets     = [0,1,0]
delta = 1

residuals   = [2,-1,-2]
losses      = [1.5,0.5,1.5]
mean loss   = 7/6 ≈ 1.166666667
gradients   = [1/3,-1/3,-1/3]
```

## Shared contract

- Nonempty, equal-length vectors of finite real predictions and
  finite real targets. Threshold `delta` must be finite and
  **strictly greater than zero**.
- Return a nonnegative finite **mean loss** and gradient vector
  of the same length as the inputs.
- Validate every input and every arithmetic intermediate, including
  prediction-target subtraction, squared residuals, linear term,
  aggregate loss and gradient values. Reject nonfinite results
  instead of silently clipping residuals.
- C accepts `double predictions[n]`, `double targets[n]`,
  `double delta`, and caller-owned `double* mean_loss` plus
  `double gradients[n]` with nonoverlapping memory. Both outputs
  remain **unchanged on failure**. Rust returns
  `Result<(f64,Vec<f64>),...>`; Python returns
  `(float,list[float])`; R returns a named list.
- Python rejects booleans as numeric values. Cross-language tiny
  differences from floating-point rounding are expected.
- The gradient is with respect to predictions, not network weights.
  A model may backpropagate it through its own parameters using
  the chain rule.

## Complexity

**O(n)** time and O(n) gradient output storage. C uses an
additional scratch gradient buffer to keep output changes atomic.

## Implementations

| Language | Directory |
| --- | --- |
| C | [huber_loss_c](huber_loss_c/) |
| Python | [huber_loss_python](huber_loss_python/) |
| Rust | [huber_loss_rust](huber_loss_rust/) |
| R | [huber_loss_r](huber_loss_r/) |

Run the tests from repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover the worked example, quadratic and linear branches,
positive and negative threshold boundaries, exact fit,
invalid or empty arrays, delta bounds, and finite-input overflow.

## Limitations

This reference only evaluates a batch of scalar regression
predictions. No sample weights, reduction mode alternatives,
multitask vector targets, regularization, optimizer integration,
autodifferentiation framework or GPU training kernel.
Huber loss reduces the influence of outliers, but choosing delta
still depends on noise scale and task requirements.

## References

Huber, P. J. (1964). Robust estimation of a location parameter.
*The Annals of Mathematical Statistics, 35*(1), 73–101.
https://doi.org/10.1214/aoms/1177703732
