# Huber Loss

## Why it belongs in this repository

Huber loss balances squared error for small residuals with linear
growth for large residuals. This provides a regression objective
that is less sensitive to outliers than mean squared error while
remaining smooth at the transition (Huber, 1964).

## Mathematical definition

For residual r = actual - predicted and delta > 0:

    L_delta(r) = 0.5 * r^2                    if |r| <= delta
                 delta * (|r| - 0.5*delta)    otherwise

Return the arithmetic mean of the per-observation losses.
For delta=1, actual=[1.5,4], predicted=[1,2], losses are
0.125 and 1.5, and the mean is 0.8125.

## Contract

- Inputs: equally sized, nonempty vectors of finite real observations
  and predictions, and a positive finite delta (default 1).
- Output: finite nonnegative scalar. Zero indicates a perfect fit.
- Reject invalid lengths, Boolean/nonreal/nonfinite inputs, invalid
  delta, unrepresentable residuals and individual loss overflow.
- Python raises ValueError; R stops with error; Rust returns Err;
  C returns false and leaves its output unchanged. C input pointers
  must address n doubles and output must be writable/non-null.
- An online running average avoids overflow in the *sum* when
  individual losses are finite.

## Complexity

O(n) time and O(1) auxiliary space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [huber_loss_c](huber_loss_c/) |
| Python | [huber_loss_python](huber_loss_python/) |
| Rust | [huber_loss_rust](huber_loss_rust/) |
| R | [huber_loss_r](huber_loss_r/) |

## Tests and examples

    huber([1.5, 4], [1, 2], delta=1)   = 0.8125
    huber([1.5, 4], [1, 2], delta=2)   = 1.0625
    huber([1, 2], [1, 2], delta=1)     = 0

Run from the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Unweighted, scalar mean loss only; this does not compute gradients,
vector-Jacobian products, sample weights or multidimensional reductions.
Extremely large predictions may lead to unrepresentable finite-precision
residuals and are rejected rather than clipped.

## Reference

Huber, P. J. (1964). Robust estimation of a location parameter.
*Annals of Mathematical Statistics, 35*(1), 73–101.
https://doi.org/10.1214/aoms/1177703732
