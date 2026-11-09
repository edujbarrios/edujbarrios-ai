# Non-Overlapping 2×2 Average Pooling for CNNs

## Context

Average pooling reduces spatial dimensions while retaining the local mean activation. It complements [max_pool2d](../max_pool2d/README.md), which selects a maximum, and [global_average_pool2d](../global_average_pool2d/README.md), which reduces the entire spatial extent (Goodfellow et al., 2016).

## Definition

For row-major input `X` of height H and width W, stride-2 2×2 windows produce:

```text
Y[r,c] = (X[2r,2c] + X[2r,2c+1] + X[2r+1,2c] + X[2r+1,2c+1]) / 4
output_height = floor(H/2)
output_width  = floor(W/2)
```

There is **no padding**: if a dimension is odd, its final incomplete row or column is dropped, matching `max_pool2d`. Negative values are averaged normally without rectification.

## Contract

- Input: a flat row-major vector of exactly H×W finite real values in **[-1000,1000]**; dimensions are integers **2..32**.
- Output: a new flat row-major vector of length `floor(H/2)*floor(W/2)`. Inputs remain unchanged; no channels or batch axis are supported.
- C accepts `const double *image`, dimensions and a **distinct**, caller-owned `double *output` with sufficient capacity. Invalid dimensions, null pointers or any invalid pixel cause `false` without changing output.
- Python accepts a list/tuple and returns `list[float]`; Rust returns `Result<Vec<f64>,&str>`; R accepts a flat numeric vector and returns one numeric vector. Python/R reject booleans, malformed arrays, missing, NaN and infinity.
- This is fixed 2×2 averaging, not adaptive or global pooling.

## Complexity

**O(HW)** time; output memory **O(floor(H/2)*floor(W/2))** and constant additional space in C.

## Examples

```text
4×4 input:  1  2  3  4     result:  3.5  5.5
            5  6  7  8             11.5 13.5
            9 10 11 12
           13 14 15 16

3×5 input [1..15] => [4,6] (last row/column dropped)
2×2 input [-4,-2,-6,-8] => [-5]
```

## Implementations and tests

| Language | Implementation |
| --- | --- |
| C | [average_pool2d_c](average_pool2d_c/) |
| Python | [average_pool2d_python](average_pool2d_python/) |
| Rust | [average_pool2d_rust](average_pool2d_rust/) |
| R | [average_pool2d_r](average_pool2d_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` at repository root. Tests include odd/rectangular windows, negative activations, constant inputs, checkerboards, 32×32 bounds, and invalid values.

## Limitations

This reference does not support channels/batches, gradients, configurable pooling size/stride, partial border windows, GPUs or learned weights. Average pooling discards fine spatial detail.

## References

Goodfellow, I., Bengio, Y., & Courville, A. (2016). *Deep learning*. MIT Press. https://www.deeplearningbook.org/
