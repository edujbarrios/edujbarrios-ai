# Mean Squared Error (MSE)

## Why it belongs in this repository

Mean squared error is a common regression evaluation metric and training
objective. Squaring residuals makes large prediction errors count more
heavily than small ones (Hastie et al., 2009). It complements this catalogue's
mean absolute error implementation.

## Mathematical definition

For n paired observations, with actual values y and predictions y_hat:

    MSE(y, y_hat) = (1 / n) * sum_i (y_i - y_hat_i)^2

The output is nonnegative, and zero means perfect predictions.

## Contract

- Inputs: two nonempty, equal-length sequences of **finite real numbers**.
- Output: one finite, nonnegative floating-point scalar.
- Invalid inputs: empty or unequal lengths, Boolean/nonreal/nonfinite
  values, or any residual/square that overflows floating-point range.
- Python raises ValueError. R throws an error. Rust returns Err. C returns
  false, keeping the supplied output unchanged. C input and output pointers
  must be valid; input arrays each contain n double elements.
- A running mean of squared residuals is used to avoid overflowing the
  *sum* when all individual squared residuals are representable.

## Complexity

O(n) time and O(1) auxiliary space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [mean_squared_error_c](mean_squared_error_c/) |
| Python | [mean_squared_error_python](mean_squared_error_python/) |
| Rust | [mean_squared_error_rust](mean_squared_error_rust/) |
| R | [mean_squared_error_r](mean_squared_error_r/) |

## Examples and tests

    MSE([3, 0], [1, 2]) = 4
    MSE([1, 2], [1, 2]) = 0
    MSE([1, 2], [2, 3]) = 1

Run the repository test commands:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is an unweighted scalar MSE, not root MSE, a tensor reduction, or a
streaming estimator over arbitrarily large unbounded values. An individual
unrepresentable squared residual causes an error, even if a mathematical
average would otherwise be finite. MSE is sensitive to outliers.

## References

Hastie, T., Tibshirani, R., & Friedman, J. (2009). *The elements of
statistical learning: Data mining, inference, and prediction* (2nd ed.).
Springer. https://doi.org/10.1007/978-0-387-84858-7
