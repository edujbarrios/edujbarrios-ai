# Soft Dice Loss for Binary Segmentation

## Why it belongs in this repository

Soft Dice loss provides a differentiable overlap-oriented objective for training image segmentation networks. It extends the catalogue's hard binary Dice overlap metric to **probabilistic** pixel predictions, which are typical during neural-network training. Overlap objectives are useful for class-imbalanced segmentation (Milletari et al., 2016).

## Mathematical definition

For n predicted foreground probabilities p[i] in [0,1] and ground-truth mask labels t[i] in {0,1}, compute with fixed additive smoothing epsilon=1:

    overlap = sum_i p[i] * t[i]
    soft_dice = (2*overlap + 1) / (sum_i p[i] + sum_i t[i] + 1)
    loss = 1 - soft_dice

Using epsilon=1 makes all-empty predictions and masks have **zero loss**. The result is in [0,1] subject to normal floating-point roundoff; it is not the same as hard Dice on thresholded predictions.

## Contract

- Input: equally long one-dimensional arrays of length 1..512.
- Predictions: finite real values in [0,1]; targets: exact integer labels 0 or 1.
- Output: a nonnegative scalar soft Dice loss. No input mutation.
- C accepts `const double *predictions`, `const uint8_t *targets`, dimension n and caller output pointer; errors return false without modifying output.
- Python accepts list/tuple predictions with numeric non-boolean elements and list/tuple targets of plain integer labels; raises ValueError for invalid inputs.
- Rust accepts f64 prediction and u8 target slices and returns Option<f64>.
- R accepts one-dimensional numeric vectors, with targets restricted to integer-valued zeros and ones; invalid inputs raise error.
- All calculations use binary64. Up to 512 observations prevent accumulator overflow under the probability bounds.

## Complexity

O(n) time and O(1) auxiliary space.

## Implementations

| Language | Folder |
| --- | --- |
| C | [soft_dice_loss_c](soft_dice_loss_c/) |
| Python | [soft_dice_loss_python](soft_dice_loss_python/) |
| Rust | [soft_dice_loss_rust](soft_dice_loss_rust/) |
| R | [soft_dice_loss_r](soft_dice_loss_r/) |

## Examples and tests

- p=[0,0], t=[0,0]: loss 0.
- p=[1,0], t=[1,0]: loss 0.
- p=[0,1], t=[1,0]: loss 2/3.
- p=[0.5,0.5], t=[1,0]: loss 1/3.
- p=[1,1], t=[0,0]: loss 2/3.

Tests also cover singleton inputs, 512 observations, invalid probabilities, label values and mismatched dimensions.

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

## Limitations

This is a **single binary foreground channel** and a fixed smoothing convention. It does not implement multiclass Dice, per-image batch reduction, logits-to-probabilities conversion, class weighting, threshold selection or explicit backpropagation. Different smoothers and loss conventions yield different numerical values.

## References

Milletari, F., Navab, N., & Ahmadi, S.-A. (2016). V-Net: Fully convolutional neural networks for volumetric medical image segmentation. In *2016 Fourth International Conference on 3D Vision (3DV)* (pp. 565–571). IEEE. https://doi.org/10.1109/3DV.2016.79
