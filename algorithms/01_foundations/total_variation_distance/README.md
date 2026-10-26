# Total Variation Distance

## Why it belongs in this repository

Total variation (TV) distance is a symmetric measure of how different two
probability distributions are. It is useful in probabilistic modeling,
distribution shift detection, and the study of stochastic algorithms
(Cover & Thomas, 2006).

## Mathematical definition

For two distributions over the same finite events:

    TV(p, q) = (1/2) * sum_i |p_i - q_i|

It equals zero for identical distributions and one for disjoint supports.

## Contract

- Inputs: two equally long, **nonempty** arrays of finite real probabilities
  in [0,1]. Each array's sum must differ from 1 by no more than 1e-9.
- Output: one floating-point number in [0,1]. Slight rounding excursions
  outside that range are clamped.
- Empty, mismatched, nonnumeric, nonfinite, negative or unnormalized inputs
  are rejected. Python additionally rejects Boolean values. R accepts
  one-dimensional real numeric vectors; C and Rust use f64/double arrays.
- C returns a success Boolean and does not change the output on failure.
  The input pointers must each address n elements; all pointers must be
  nonnull, and the output must be separately writable.
- This implementation **does not silently normalize** the inputs.

## Complexity

O(n) time and O(1) auxiliary space for n event categories.

## Implementations

| Language | Folder |
| --- | --- |
| C | [total_variation_distance_c](total_variation_distance_c/) |
| Python | [total_variation_distance_python](total_variation_distance_python/) |
| Rust | [total_variation_distance_rust](total_variation_distance_rust/) |
| R | [total_variation_distance_r](total_variation_distance_r/) |

## Examples and tests

    TV([0.5,0.5], [0.5,0.5]) = 0
    TV([1,0], [0,1])         = 1
    TV([1,0], [0.25,0.75])   = 0.75

Run the repository checks from its root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is a reference implementation for discrete, already-normalized vectors;
it does not estimate densities, align event labels or handle continuous
distributions. Floating-point tolerance can admit sums that are very slightly
different from one.

## References

Cover, T. M., & Thomas, J. A. (2006). *Elements of information theory*
(2nd ed.). Wiley.
