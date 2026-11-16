# Discrete Hough Line Transform

## Why it belongs in AI

The Hough transform finds global geometric structure from local edge
pixels. In a computer-vision pipeline, it can identify straight line
candidates despite gaps or noisy edge detections. The standard
rho–theta formulation is described by Duda and Hart (1972).

## Algorithm

For a foreground edge pixel at image coordinate `(x,y)` and
a line-normal angle `theta`, a compatible line satisfies

```text
rho = x * cos(theta) + y * sin(theta)
```

Here `x` is the **column index**, `y` the **row index**,
and both increase rightward/downward from zero.

1. For each edge pixel with value 1, iterate every configured normal
   angle in degrees.
2. Quantize `rho` to the integer bin
   `round(rho / rho_step)` using Python's round-to-nearest,
   ties-to-even convention.
3. Add one vote to that `(angle_index, rho_bin)` accumulator cell.
4. Return up to `top_k` nonempty bins ranked by **descending votes**,
   then **angle input order**, then **ascending rho bin**.

The result is `(angle_degrees, rho_bin * rho_step, votes)`.
An angle of 0 degrees represents a vertical line (`x=rho`);
90 degrees represents a horizontal line (`y=rho`).

## Contract

- `hough_line_transform(edges, angles=(0,45,90,135),
  rho_step=1.0, top_k=5)` accepts a binary matrix of
  **1..128 rows** and **1..128 columns**, each value exactly
  integer 0 or 1 (booleans rejected).
- `angles` must be a list/tuple of **1..180 distinct** finite,
  non-boolean real degrees in `[0,180)`. The original order
  is significant for tie-breaking.
- `rho_step` must be finite in `[0.001,1000]` and `top_k`
  an integer in `[1,1024]`; booleans are rejected.
- If there are no foreground pixels, return an empty list.
  Invalid inputs raise `ValueError`. No input mutation.
  Floating computations use binary64; results depend on the
  explicitly documented rounding convention.

## Worked example

```python
edges = [[0,1,0], [0,1,0], [0,1,0]]
hough_line_transform(edges, angles=[0,90], top_k=3)
# [(0.0, 1.0, 3), (90.0, 0.0, 1), (90.0, 1.0, 1)]
```

Three foreground pixels vote for the same vertical line `x=1`.
The other two results are lower-ranked horizontal candidates.

## Complexity

For image height `H`, width `W`, `E` edge pixels,
`A` angles and `B` distinct nonempty accumulator bins,
time is **O(HW + EA + B log B)**.
Space is **O(B + A)** in the sparse accumulator and
validation of angles, plus O(min(B,top_k)) output.
The image is not copied.

## Implementations

| Language | Folder |
| --- | --- |
| Python | [hough_line_transform_python](hough_line_transform_python/) |

Other languages are not yet implemented.

## Examples and tests

From the repository root:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict --language python
```

Tests cover vertical/horizontal/diagonal lines, empty masks,
deterministic vote ties, selected angles, pixel validation,
parameter bounds and input immutability.

## Limitations

This is a pedagogical **sparse vote accumulator**, not a full line
detector: it does not threshold peaks, suppress nearby maxima, infer
endpoints, draw segments or run an edge detector. Coarse angles and
rho quantization can alias distinct lines. The transform detects
infinite geometric lines, not necessarily long connected segments.
It does not use a GPU or report performance benchmarks.

## References

Duda, R. O., & Hart, P. E. (1972). Use of the Hough transformation to
detect lines and curves in pictures. *Communications of the ACM, 15*(1),
11–15. https://doi.org/10.1145/361237.361242
