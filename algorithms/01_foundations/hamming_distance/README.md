# Hamming Distance for Byte Sequences

## Why it belongs in this repository

Hamming distance counts positions at which two equal-length symbol sequences
differ. In AI systems it provides a simple distance for discrete codes, binary
sketches and categorical token signatures. Hamming (1950) developed the related
foundations of error-detecting and error-correcting codes.

## What it does

For equally sized byte sequences `a` and `b`:

```text
distance(a, b) = sum(i = 1..n) [a[i] != b[i]]
```

For `[1, 0, 1, 1]` and `[1, 1, 0, 1]`, the result is `2`.
For `[0]` and `[255]`, the result is `1`, **not** eight:
this is symbol-wise Hamming distance, not differing-bit popcount.

## Contract

- Inputs: equal-length sequences of **bytes** (each symbol is 0–255).
  C takes `uint8_t` buffers, Rust takes `u8` slices,
  Python accepts `bytes` or `bytearray`, R accepts `raw` vectors.
- Output: the number of differing positions, an integer between 0 and n.
- Both empty sequences are valid and return 0. Differing lengths are invalid.
- C returns `false` for unequal lengths, null output, or null buffers when
  n > 0; on failure the output is unchanged. Python/R raise an error and
  Rust returns `Err`.
- This is **substitution-only** distance. Insertions, deletions and alignment
  are deliberately excluded.

## Complexity

**O(n)** time and **O(1)** auxiliary space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [hamming_distance_c](hamming_distance_c/) |
| Python | [hamming_distance_python](hamming_distance_python/) |
| Rust | [hamming_distance_rust](hamming_distance_rust/) |
| R | [hamming_distance_r](hamming_distance_r/) |

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Reference cases cover two substitutions, identical sequences, both empty,
the 0-versus-255 distinction, unequal lengths and invalid input types.

## Limitations

Inputs must be pre-aligned. This implementation does not normalize the
distance, count differing bits within a byte, search a large index, or support
Unicode code points. A byte-encoded text representation may not correspond
to one linguistic character per byte.

## References

Hamming, R. W. (1950). Error detecting and error correcting codes.
*Bell System Technical Journal, 29*(2), 147–160.
https://doi.org/10.1002/j.1538-7305.1950.tb00463.x
