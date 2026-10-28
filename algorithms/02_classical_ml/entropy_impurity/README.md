# Decision Tree Entropy Impurity

## Why it belongs here

Classification trees can evaluate candidate node splits using
**class entropy**, an impurity measure reflecting how mixed a class
distribution is. It complements the catalogue's Gini impurity
implementation; information gain is the reduction in entropy
achieved by a split (Quinlan, 1986).

## Mathematical definition

For nonnegative integer class counts `c[0], ..., c[k-1]`, let
`N = sum(c)` and `p[i] = c[i] / N`. For `N > 0`,

```text
H = -sum(p[i] * log2(p[i]) for c[i] > 0)
```

Zero-count categories contribute exactly zero (the usual
`0 log 0 = 0` convention). Results are in **bits**, not nats.

Examples: `[5,0]` → `0`, `[1,1]` → `1`,
`[1,1,1,1]` → `2`, `[3,1]` → approximately
`0.8112781244591328`.

## Contract

- `1 <= k <= 128` class counts.
- Every count is an integer in `[0,1,000,000]`; at least
  one count must be positive. The input is invalid otherwise.
- The output is a finite scalar entropy in `[0, log2(k)]`.
- C accepts `uint32_t[]`, a count length and an output
  pointer; it returns false on failure and does **not** alter
  the output on failure. Output must not overlap input.
- Rust accepts `&[u32]` and returns `Result<f64, &str>`.
  Python accepts lists/tuples of integers (not bools).
  R accepts a plain numeric vector of integer-valued entries,
  not a matrix or complex vector.
- This algorithm measures impurity for a *single* node and
  does not select a split or train a decision tree.

## Complexity

**O(k)** time and **O(1)** auxiliary space.

## Implementations and tests

| Language | Folder |
| --- | --- |
| C | [entropy_impurity_c](entropy_impurity_c/) |
| Python | [entropy_impurity_python](entropy_impurity_python/) |
| Rust | [entropy_impurity_rust](entropy_impurity_rust/) |
| R | [entropy_impurity_r](entropy_impurity_r/) |

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests check pure, balanced, imbalanced and multivariate class
distributions, one class, boundary counts, zeros and invalid
types/ranges. Cross-language numeric assertions use a tolerance.

## Limitations

No weighted counts, continuous labels, statistical bias correction
or information-gain split selection. Finite floating-point roundoff
may change final digits.

## References

- Quinlan, J. R. (1986). Induction of decision trees.
  *Machine Learning, 1*, 81–106.
  https://doi.org/10.1007/BF00116251
