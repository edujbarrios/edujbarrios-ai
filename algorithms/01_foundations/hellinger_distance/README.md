# Hellinger Distance

## Why it belongs in this repository

Hellinger distance is a symmetric, bounded way to compare discrete
probability distributions. Unlike KL divergence, it stays finite when
one distribution assigns zero probability to an event (Hellinger, 1909).
It is useful in distribution shift detection and probabilistic ML.

## Mathematical definition

For equal-length probability vectors p and q:

    H(p,q) = sqrt( 1/2 * sum_i (sqrt(p_i) - sqrt(q_i))^2 )

The result is in [0,1]. Matching distributions yield zero; disjoint
probability mass yields one.

## Contract

- Inputs: equally sized, nonempty vectors of finite, nonnegative real
  probabilities. Each vector must sum to 1 within an absolute tolerance
  of 1e-9. Empty vectors and unequal lengths are invalid.
- Python rejects Boolean/non-numeric values with ValueError; R rejects
  nonnumeric, complex, nonfinite and matrix inputs; typed C and Rust
  inputs must be finite, nonnegative doubles.
- Python raises ValueError, R raises an error, Rust returns Err and C
  returns false on invalid input. C leaves its output unchanged on
  failure; all pointers must reference their declared lengths.
- Finite precision may yield tiny roundoff differences. The final result
  is clamped to [0,1] to handle small rounding excursions.

## Complexity

O(n) time and O(1) auxiliary space for n categories, not counting inputs.

## Implementations

| Language | Folder |
| --- | --- |
| C | [hellinger_distance_c](hellinger_distance_c/) |
| Python | [hellinger_distance_python](hellinger_distance_python/) |
| Rust | [hellinger_distance_rust](hellinger_distance_rust/) |
| R | [hellinger_distance_r](hellinger_distance_r/) |

## Examples and tests

    H([0.5,0.5], [0.5,0.5]) = 0
    H([1,0], [0,1])         = 1
    H([1,0], [0.5,0.5])     = sqrt(1 - 1/sqrt(2))

Run the repository checks:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Only discrete, already normalized vectors are supported. No smoothing,
density estimation, continuous-distribution integration or GPU acceleration
is performed. This is a pedagogical reference implementation.

## References

Hellinger, E. (1909). Neue Begründung der Theorie quadratischer Formen von
unendlichvielen Veränderlichen. *Journal für die reine und angewandte
Mathematik, 136*, 210–271.
