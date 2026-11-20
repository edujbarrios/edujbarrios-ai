# Binary Mutual Information

## Why it belongs in AI

Mutual information measures the information shared by two random variables
and is used in feature selection and dependency analysis. Unlike correlation,
it can capture general statistical dependence (Cover & Thomas, 2006).

This implementation handles **binary variables**, computing the plug-in
(empirical) mutual information from a 2-by-2 contingency table without
probability smoothing.

## Definition

Given n paired observations x and y with values in {0, 1}, let c_ab
count the pairs (x=a, y=b), and define p_ab = c_ab / n. Let p_a* and
p_*b be the empirical marginal probabilities. Then

    I(X;Y) = sum_{a,b in {0,1}, p_ab > 0}
                 p_ab * log2(p_ab / (p_a* * p_*b)).

Zero-probability joint cells contribute zero and are never divided.
The result is measured in **bits** and mathematically ranges from
0 to 1 for two binary variables.

Examples:
- x=[0,0,1,1], y=[0,0,1,1] -> 1 bit (perfect association).
- x=[0,0,1,1], y=[0,1,0,1] -> 0 bits (independent in the sample).
- x=[0,0,0], y=[1,0,1] -> 0 bits (constant x).

## Contract

- Inputs: nonempty, equal-length vectors of binary integer observations
  (each element exactly 0 or 1); R accepts numeric binary 0/1 values.
- Output: one double-precision mutual information value in bits.
- Invalid: empty or mismatched vectors, values outside {0,1}, missing
  or non-finite values, and non-integer values. Python rejects booleans
  explicitly; C/Rust use integer element types.
- C writes to a nonoverlapping caller-owned scalar and **does not modify
  it on failure**. Returned finite values are clamped at zero only to
  handle tiny negative numerical roundoff.
- No randomization or logarithm base conversion is necessary.

## Complexity

O(n) time for counting, O(1) auxiliary space (besides input storage).

## Implementations

| Language | Directory |
| --- | --- |
| C | [binary_mutual_information_c](binary_mutual_information_c/) |
| Python | [binary_mutual_information_python](binary_mutual_information_python/) |
| Rust | [binary_mutual_information_rust](binary_mutual_information_rust/) |
| R | [binary_mutual_information_r](binary_mutual_information_r/) |

Run:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

Empirical MI can be upward-biased for small samples. This version
does not implement bias correction, sample weights, confidence
intervals, continuous variables, missing-data imputation or general
categorical alphabets. It describes observed dependence, not causality.

## References

Cover, T. M., & Thomas, J. A. (2006). *Elements of information theory*
(2nd ed.). Wiley-Interscience. https://doi.org/10.1002/047174882X
