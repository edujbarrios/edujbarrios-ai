#pragma once

#include <cuda_runtime.h>
#include <cstddef>

// Inputs live in host memory. Caller supplies valid arrays of at least n
// doubles. Empty arrays may be null. On error, *out is not changed.
cudaError_t dot_product_cuda(const double* left, const double* right,
                             std::size_t n, double* out);
