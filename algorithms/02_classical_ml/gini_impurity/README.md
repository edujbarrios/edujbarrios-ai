# Gini Impurity

## Why it belongs here

Classification trees use **Gini impurity** to measure class mixing
at a node and evaluate candidate splits. A pure node has impurity
zero; a balanced two-class node has impurity 0.5
(Breiman et al., 1984).

## Definition

For `k` nonnegative integer class counts `c_i` and
`N = sum(c_i) > 0`, define `p_i = c_i/N` and

```text
Gini(c) = 1 - sum_i(p_i * p_i).
```

Examples: `[5, 0]` gives 0; `[1, 1]` gives 0.5;
`[3, 1]` gives 0.375; and `[1, 1, 1]` gives 2/3.

## Contract

- Between **1 and 128** class counts. Each count must be
  a nonnegative integer no greater than **1,000,000**.
- At least one count must be positive; an empty or all-zero
  collection is invalid.
- The result is a finite double in `[0, 1]`. It is computed
  from normalized counts without requiring probability inputs.
- C takes a `uint32_t` array and writes a caller-owned output
  **only on success**; the output must not overlap the input.
  Rust uses `&[u32]` and returns `Result<f64, &str>`.
  Python takes lists/tuples of integers and rejects bools.
  R takes a numeric vector, rejecting nonintegers and matrices.
- This function does **not** choose a tree split or compute
  information gain. For one positive class it returns zero.

## Complexity

**O(k)** time and **O(1)** auxiliary memory.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [gini_impurity_c](gini_impurity_c/) |
| Python | [gini_impurity_python](gini_impurity_python/) |
| Rust | [gini_impurity_rust](gini_impurity_rust/) |
| R | [gini_impurity_r](gini_impurity_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover pure, balanced and imbalanced distributions,
multiple categories, maximum allowed counts, empty/all-zero,
negative, fractional, excessive and nonfinite inputs where
the language can represent them.

## Limitations

No weights, missing categories, split selection or tree training.
Only nonnegative count data with the declared input bounds are
supported. Double-precision rounding can affect the last bits.

## References

- Breiman, L., Friedman, J. H., Olshen, R. A., & Stone, C. J.
  (1984). *Classification and regression trees*.
  Wadsworth International Group.
