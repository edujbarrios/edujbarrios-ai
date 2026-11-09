# Group Normalization (Channel-Major Tensor)

## Neural-network motivation

**Group Normalization (GroupNorm)** divides the channels of a single sample into groups, computes separate mean/variance statistics for each group across its channels and spatial positions, then applies per-channel affine parameters. Unlike Batch Normalization it does not depend on a batch dimension; it generalizes the normalization granularity between LayerNorm-like and instance-normalization-like behavior (Wu & He, 2018).

## Mathematical definition

Let `C` be the number of channels, `S` the flattened spatial size per channel, and `G` a positive group count dividing `C`. The input is a **flat channel-major tensor**: `x[c*S + s]`, where `c` is zero-based channel index and `s` is zero-based spatial offset.

For group `g` consisting of channels `g*(C/G)..(g+1)*(C/G)-1`, define:

```text
N = (C/G)*S
mu[g] = (1/N) * sum(x[c,s] in group g)
var[g] = (1/N) * sum((x[c,s]-mu[g])^2 in group g)
y[c,s] = gamma[c] * ((x[c,s]-mu[g])/sqrt(var[g]+eps)) + beta[c]
```

This is **population variance** (divide by N, not N−1); epsilon goes **inside** the square root. Gamma/beta are provided rather than fitted. Normalization is performed independently for each group and uses no observations from another batch item. A group with constant input has normalized activation zero and outputs exactly beta for its channels.

## Shared contract

- `channels` (**C**) integer **1..32**; `spatial_size` (**S**) integer **1..64**; `groups` (**G**) integer **1..C** with `C % G == 0`.
- Input `x` is flat channel-major length exactly `C*S`; `gamma` and `beta` are flat vectors of exactly C elements. Every element is a finite real value in **[-1000,1000]**. `epsilon` is finite and in **(0,1]**.
- Output is a new flat channel-major vector of **C*S finite doubles**. No input is modified.
- C takes `const double *input`, `size_t channels,spatial_size,groups`, `const double *gamma, *beta`, `double epsilon` and a caller-owned **distinct** `double *output` of length `C*S`. C returns `false` and preserves the output if any input or intermediate value is invalid. Uses a fixed scratch buffer before committing results.
- Python accepts flat list/tuple numeric vectors and integer dimensions; returns `list[float]`, rejecting booleans, invalid shape and nonfinite data. Rust returns `Result<Vec<f64>,&str>`. R accepts flat numeric vectors and returns a numeric vector; rejects logical, complex, matrices, missing values and bad dimensions.
- This is an educational single-example, channel-major forward pass, not a learned backward pass or a batched tensor framework.

## Complexity

Every group is scanned for its mean, variance, then output. **O(C*S)** time; **O(C*S)** output memory and O(1) additional group working memory (C uses O(C*S) scratch to guarantee unchanged output on errors). Maximum input size is 2048.

## Examples

```text
C=4, S=2, G=2, x=[1,2,3,4, 5,6,7,8], gamma=[1,1,1,1],
beta=[0,0,0,0], epsilon=0.25

group means:     [2.5,6.5]
group variances: [1.25,1.25]
output = [-1.5,-0.5,0.5,1.5, -1.5,-0.5,0.5,1.5] / sqrt(1.5)

C=2, S=1, G=2, x=[10,20], gamma=[3,4], beta=[5,6]
output=[5,6]   # each group contains one activation
```

## Implementations and testing

| Language | Folder |
| --- | --- |
| C | [group_normalization_c](group_normalization_c/) |
| Python | [group_normalization_python](group_normalization_python/) |
| Rust | [group_normalization_rust](group_normalization_rust/) |
| R | [group_normalization_r](group_normalization_r/) |

Run `python3 scripts/validate_repository.py` and `python3 scripts/run_tests.py --strict` from the repository root. Tests include multi-channel grouping, affine gamma/beta, constant groups, singleton groups, maximum 32×64 tensor, invalid group division, bad shapes, nonfinite values and epsilon.

## Limitations

No batch dimension, distributed training statistics, automatic differentiation, GPU acceleration, trainable parameter updates, or streaming inference. Bounded input magnitudes simplify finite arithmetic and make this a readable reference rather than a replacement for optimized normalization kernels.

## References

Wu, Y., & He, K. (2018). Group normalization. In *Proceedings of the European Conference on Computer Vision (ECCV)* (pp. 3–19). Springer. https://doi.org/10.1007/978-3-030-01261-8_1
