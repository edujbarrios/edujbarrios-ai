# Binary Segmentation Dice Coefficient

## Why it belongs here

The Sørensen–Dice overlap coefficient measures the agreement between
predicted and reference foreground pixels in binary segmentation. It gives
a simple, interpretable complement to binary intersection over union (IoU);
the corresponding set-overlap formulation was described by Dice (1945).

## Algorithm

For aligned binary masks `prediction` and `target`:

```text
intersection = number of positions where both masks contain 1
foreground_a = number of ones in prediction
foreground_b = number of ones in target
Dice = 2 * intersection / (foreground_a + foreground_b)
```

When both masks have no foreground, return **1** by explicit convention.
For `[1,1,0,0]` and `[1,0,1,0]`, Dice is `0.5`.
Unlike IoU, Dice divides by the sum of foreground sizes, not their union.

## Contract

- Two nonempty, same-length, aligned sequences of binary pixel values
  (exactly 0 or 1). Position order must match in both sequences.
- Return a score in `[0,1]`; identical masks return 1, disjoint
  nonempty foreground regions return 0, and two all-zero masks return 1.
- Reject empty/mismatched masks and any nonbinary value. Python rejects
  booleans as pixels; R accepts finite numeric zeros and ones but not logicals.
- C uses byte pointers and a caller-owned output double, unchanged on error.
  Rust uses slices of `u8` and returns `Result<f64, &str>`.
  Python and R raise errors for invalid inputs.
- This is a deterministic binary score, not a differentiable soft Dice loss.

## Complexity

Time **O(n)**; extra space **O(1)** in each implementation.

## Implementations

| Language | Folder |
| --- | --- |
| C | [binary_dice_coefficient_c](binary_dice_coefficient_c/) |
| Python | [binary_dice_coefficient_python](binary_dice_coefficient_python/) |
| Rust | [binary_dice_coefficient_rust](binary_dice_coefficient_rust/) |
| R | [binary_dice_coefficient_r](binary_dice_coefficient_r/) |

## Examples and tests

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

Tests cover partial overlap, no overlap, perfect overlap, all-background
masks, unequal lengths and invalid pixels.

## Limitations

No probabilistic pixels, batching, multi-class segmentation, shape checking,
or smoothing epsilon. The both-empty-foreground convention may differ from
other evaluation protocols.

## References

Dice, L. R. (1945). Measures of the amount of ecologic association between
species. *Ecology, 26*(3), 297–302. https://doi.org/10.2307/1932409
