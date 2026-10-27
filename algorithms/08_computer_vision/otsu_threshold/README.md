# Otsu Global Grayscale Threshold

## Why it matters for computer vision

Otsu's method automatically determines an intensity threshold for
binarizing grayscale images by maximizing the variance between the
foreground and background classes (Otsu, 1979). It is commonly
used for classical image segmentation before further analysis.

## Algorithm

Build a histogram `hist[0..255]` for the input 8-bit grayscale
pixels. For each candidate threshold `t=0..254` with at least
one observation in both classes (pixels `<=t` vs `>t`), compute

```text
w0 = count(values <= t)
w1 = N - w0
sum0 = sum(values <= t)
sumall = sum(values)
score = (sumall*w0 - N*sum0)^2 / (w0*w1)
```

This quantity is proportional to the between-class variance;
the omitted positive normalization factor is the same for every
threshold. Choose the threshold with the **greatest score**,
breaking ties by the **smallest threshold**. If all pixels have
the same value, there are no two nonempty classes; return `0`
by convention.

Example: `[10,20,200,210]` returns threshold `20`,
and `[0,0,255,255]` returns `0`.

## Contract

- Input is `1..4096` integer-valued pixels between `0` and
  `255`, in any flattening order; shape is not needed for the
  global threshold.
- Python rejects boolean, float and nonintegral pixel values;
  R accepts numeric integer-valued vectors and rejects NA,
  NaN, Inf or fractions. C/Rust take unsigned 8-bit pixels.
- Returns a single integer threshold in `[0,254]`;
  constant images yield `0` by the documented convention.
- Empty and overly long arrays or invalid pixel values cause errors.
  C writes to a distinct caller-owned threshold location which is
  unchanged on invalid length or null input. Rust returns
  `Result<u8,&str>`; Python returns int and R an integer.

## Complexity

A single histogram pass and 255 threshold evaluations take
**O(N+256)** time and **O(256)** extra space. The reference
implementations do not need image dimensions or external packages.

## Implementations

| Language | Folder |
| --- | --- |
| C | [otsu_threshold_c](otsu_threshold_c/) |
| Python | [otsu_threshold_python](otsu_threshold_python/) |
| Rust | [otsu_threshold_rust](otsu_threshold_rust/) |
| R | [otsu_threshold_r](otsu_threshold_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests include separable bimodal pixels, constant images,
adjacent intensity values, ties, invalid lengths and
out-of-range/nonintegral input.

## Limitations

The returned value is a global threshold, not an already binarized
image. If using it for segmentation, callers can classify each
pixel as `value > threshold`. Strong illumination gradients,
overlapping intensity distributions and noise can make a single
global threshold ineffective.

## References

- Otsu, N. (1979). A threshold selection method from gray-level histograms. *IEEE Transactions on Systems, Man, and Cybernetics, 9*(1), 62–66. https://doi.org/10.1109/TSMC.1979.4310076
