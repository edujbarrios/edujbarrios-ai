# Channel-Wise Global Average Pooling

## CNN context

Global average pooling reduces each spatial feature map to one feature
per channel, often replacing large dense classification heads in CNNs
(Lin et al., 2014). This reference operates on a single channel-first
CHW tensor without a batch dimension.

## Definition

For `C` channels, `H` rows and `W` columns, the input
is a flat array in **channel-first row-major** order:

```text
y[c] = (1/(H*W)) * sum_{r=0}^{H-1} sum_{s=0}^{W-1}
       x[c*H*W + r*W + s]
```

Example: `C=2,H=W=2`, `x=[1,2,3,4, 5,6,7,8]` yields
`y=[2.5,6.5]`. A single-pixel map returns its own value.

## Contract

- `1<=C<=16` and `1<=H,W<=32`.
- Input must contain exactly `C*H*W` real-valued elements.
  Every value must be finite and lie in `[-1000,1000]`.
- Output contains exactly `C` finite values in original
  channel order; negative values are allowed.
- C requires a separate caller-owned `C`-element output that
  is unchanged on validation failure. Rust returns
  `Result<Vec<f64>,&str>`; Python and R return numeric vectors.
  Python rejects booleans as inputs and dimensions.

## Complexity

**O(CHW)** time and **O(C)** output memory, with constant
additional scratch space in C.

## Implementations

| Language | Folder |
| --- | --- |
| C | [global_average_pool2d_c](global_average_pool2d_c/) |
| Python | [global_average_pool2d_python](global_average_pool2d_python/) |
| Rust | [global_average_pool2d_rust](global_average_pool2d_rust/) |
| R | [global_average_pool2d_r](global_average_pool2d_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover multiple channels, a singleton map, negative
activations, invalid dimensions, nonfinite values and bad lengths.

## Limitations

This is an educational forward operator, not a CNN classifier.
It does not implement masked pooling, batch tensors, gradient
calculation or optimized GPU kernels.

## References

- Lin, M., Chen, Q., & Yan, S. (2014). *Network in network*. International Conference on Learning Representations. https://arxiv.org/abs/1312.4400
