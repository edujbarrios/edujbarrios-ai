# Dot Product

**Stage:** 01 — Foundations  
**Implementations:** C, Python, Rust, R and optional NVIDIA CUDA.

## Why it is here

Dot products appear in linear models, dense neural network layers, embedding
similarity and query/key attention scores. This primitive builds toward
cosine similarity, matrix multiplication, search and retrieval algorithms.

## What it does

For equal-size vectors, multiply corresponding components and sum:

    dot(a, b) = sum(a[i] * b[i]) for i = 0 ... n-1

    a = [1, 2, 3]
    b = [4, 5, 6]
    result = 1*4 + 2*5 + 3*6 = 32

## Shared behavior and error contract

- Two equally sized vectors of **finite real numbers**; output is a finite scalar.
- The dot product of two empty vectors is 0.0.
- Negative entries and zeros are allowed.
- Reject NaN, infinity, non-finite intermediate products and result overflow.
- Python/R/Rust detect length mismatch. The C and CUDA APIs receive one length
  plus raw pointers; the caller must guarantee each allocation has that many
  elements. Their APIs cannot infer a buffer's true length.
- Numerical rounding can differ by language, compiler and hardware. The tests
  enforce the same mathematical results within appropriate precision.

C returns `bool` and writes an output parameter *only on success*. Python
raises `ValueError`. Rust returns `Result<f64, &'static str>`. R raises an error.
CUDA returns `cudaError_t`, with the same output-on-success rule. Nonempty
CUDA inputs live in host memory and are copied to a compatible NVIDIA GPU.

## Complexity

All implementations do O(n) arithmetic. C, Python and Rust use O(1)
additional memory. R's vector multiplication allocates O(n) temporary
memory. CUDA allocates O(n) device inputs and O(number of blocks) partials,
plus host/device transfers. CUDA is **not guaranteed faster** for small data.

## Source and tests

| Language | Directory | Notes |
| --- | --- | --- |
| C | [dot_product_c/](dot_product_c/) | C11 |
| Python | [dot_product_python/](dot_product_python/) | Python 3.10+; standard library |
| Rust | [dot_product_rust/](dot_product_rust/) | Rust 2021 |
| R | [dot_product_r/](dot_product_r/) | Rscript; base R |
| CUDA | [dot_product_cuda/](dot_product_cuda/) | Optional GPU / nvcc |

Run from the repository root:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

CUDA is opt-in and requires GPU hardware:

    python3 scripts/run_tests.py --strict --with-cuda

Or run from this algorithm directory:

    cc -std=c11 dot_product_c/dot_product.c dot_product_c/test_dot_product.c -lm -o /tmp/dot_c && /tmp/dot_c
    python3 -m unittest discover -s dot_product_python -p 'test_*.py' -v
    rustc --edition=2021 --test dot_product_rust/dot_product.rs -o /tmp/dot_rs && /tmp/dot_rs
    Rscript dot_product_r/test_dot_product.R
    nvcc -std=c++17 dot_product_cuda/dot_product.cu dot_product_cuda/test_dot_product.cu -o /tmp/dot_gpu && /tmp/dot_gpu

## Limitations

These are compact reference implementations, not a tensor library. They do not
provide broadcasting, automatic differentiation, compensated summation, SIMD
or production performance guarantees. Any future benchmark must report
input sizes, hardware, compiler flags, transfer cost and methodology.

## References

- [Dot product — Encyclopedia of Mathematics](https://encyclopediaofmath.org/wiki/Dot_product)
- [CUDA C++ Programming Guide — NVIDIA](https://docs.nvidia.com/cuda/cuda-c-programming-guide/)

All original code is distributed under the repository's
[Apache License 2.0](../../../LICENSE).
