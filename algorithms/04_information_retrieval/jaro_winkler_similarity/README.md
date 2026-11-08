# Jaro–Winkler String Similarity

## Why it belongs in this repository

Jaro–Winkler measures short-string similarity and is useful in entity resolution, duplicate detection, and approximate name matching. Unlike edit distance, it combines matches within a positional window with transposition penalties and rewards a common prefix (Winkler, 1990).

## What it does

For strings of lengths `n` and `m`, match each character against an as-yet-unmatched equal character within a window of `max(0, floor(max(n,m)/2) - 1)`. Let `M` be the number of matched characters and `T` half the number of mismatches when the ordered matched-character sequences are compared.

If `M = 0`, Jaro similarity `J = 0`. Otherwise:

```text
J = (M/n + M/m + (M-T)/M) / 3
```

With `L` equal to the length of the common prefix capped at four characters, apply the Winkler boost only when `J > 0.7`:

```text
JW = J + 0.1 * L * (1 - J)  if J > 0.7
JW = J                      otherwise
```

These are deliberate fixed parameters for this implementation; variants of Jaro–Winkler exist.

## Contract

- Inputs are two printable ASCII strings (U+0020–U+007E), **0–512 characters** each. Comparison is case-sensitive; no implicit trimming or normalization.
- Returns a `double` / `float` in **[0, 1]**, where 1 means exact agreement and 0 indicates no matched characters.
- Two empty strings return **1.0**; exactly one empty string returns **0.0**.
- Reject invalid types (Python/R), nonprintable or non-ASCII characters, and strings longer than 512. C accepts NUL-terminated strings (embedded NUL is not representable).
- C returns a status `bool` and only modifies `double *score_out` on success; Rust returns `Result<f64, &str>`, Python returns `float`, and R returns a numeric scalar.
- Tests use an absolute floating-point tolerance of `1e-12`.

## Complexity

The straightforward matched-character window scan costs **O(nm)** worst-case and **O(n + m)** auxiliary memory for the match flags. No randomness, trained model, or external dependencies.

## Implementations

| Language | Folder |
| --- | --- |
| C | [jaro_winkler_similarity_c](jaro_winkler_similarity_c/) |
| Python | [jaro_winkler_similarity_python](jaro_winkler_similarity_python/) |
| Rust | [jaro_winkler_similarity_rust](jaro_winkler_similarity_rust/) |
| R | [jaro_winkler_similarity_r](jaro_winkler_similarity_r/) |

## Examples and tests

```text
similarity("MARTHA", "MARHTA")      ≈ 0.961111111111
similarity("DIXON", "DICKSONX")     ≈ 0.813333333333
similarity("JELLYFISH", "SMELLYFISH") ≈ 0.896296296296
similarity("", "")                  = 1.0
similarity("abc", "")               = 0.0
```

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover canonical comparisons, symmetry, identical strings, empty strings, mismatches, the prefix threshold, 512-character inputs, and invalid strings.

## Limitations

Not a metric distance, not guaranteed to satisfy the triangle inequality. Scores reflect spelling resemblance, not semantic meaning. The implementation is ASCII-only and deliberately does not use locale-aware case folding, Unicode grapheme clusters, phonetic similarity, learned weights, or configurable thresholds.

## References

Winkler, W. E. (1990). String comparator metrics and enhanced decision rules in the Fellegi–Sunter model of record linkage. *Proceedings of the Section on Survey Research Methods*, 354–359. American Statistical Association. https://www.asasrms.org/Proceedings/papers/1990_056.pdf
