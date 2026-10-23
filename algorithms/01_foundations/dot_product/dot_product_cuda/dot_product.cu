#include "dot_product.cuh"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

namespace {
constexpr unsigned int kThreads = 256;

__global__ void dot_kernel(const double* left, const double* right,
                           std::size_t n, double* partials) {
    __shared__ double local[kThreads];
    const unsigned int lane = threadIdx.x;
    const std::size_t start =
        static_cast<std::size_t>(blockIdx.x) * blockDim.x + lane;
    const std::size_t stride =
        static_cast<std::size_t>(gridDim.x) * blockDim.x;
    double total = 0.0;
    for (std::size_t i = start; i < n; i += stride) {
        total += left[i] * right[i];
    }
    local[lane] = total;
    __syncthreads();

    for (unsigned int offset = blockDim.x / 2; offset > 0; offset /= 2) {
        if (lane < offset) {
            local[lane] += local[lane + offset];
        }
        __syncthreads();
    }
    if (lane == 0) {
        partials[blockIdx.x] = local[0];
    }
}
}  // namespace

cudaError_t dot_product_cuda(const double* left, const double* right,
                             std::size_t n, double* out) {
    if (out == nullptr || (n > 0 && (left == nullptr || right == nullptr))) {
        return cudaErrorInvalidValue;
    }
    if (n == 0) {
        *out = 0.0;
        return cudaSuccess;
    }
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(double)) {
        return cudaErrorInvalidValue;
    }
    for (std::size_t i = 0; i < n; ++i) {
        if (!std::isfinite(left[i]) || !std::isfinite(right[i])) {
            return cudaErrorInvalidValue;
        }
    }

    const std::size_t blocks = std::min<std::size_t>(128, 1 + (n - 1) / kThreads);
    const std::size_t bytes = n * sizeof(double);
    double* dev_left = nullptr;
    double* dev_right = nullptr;
    double* dev_partials = nullptr;

    auto clean_up = [&]() {
        if (dev_left != nullptr) cudaFree(dev_left);
        if (dev_right != nullptr) cudaFree(dev_right);
        if (dev_partials != nullptr) cudaFree(dev_partials);
    };
    cudaError_t status = cudaMalloc(reinterpret_cast<void**>(&dev_left), bytes);
    if (status != cudaSuccess) { clean_up(); return status; }
    status = cudaMalloc(reinterpret_cast<void**>(&dev_right), bytes);
    if (status != cudaSuccess) { clean_up(); return status; }
    status = cudaMalloc(reinterpret_cast<void**>(&dev_partials),
                        blocks * sizeof(double));
    if (status != cudaSuccess) { clean_up(); return status; }

    status = cudaMemcpy(dev_left, left, bytes, cudaMemcpyHostToDevice);
    if (status != cudaSuccess) { clean_up(); return status; }
    status = cudaMemcpy(dev_right, right, bytes, cudaMemcpyHostToDevice);
    if (status != cudaSuccess) { clean_up(); return status; }

    dot_kernel<<<static_cast<unsigned int>(blocks), kThreads>>>(
        dev_left, dev_right, n, dev_partials);
    status = cudaGetLastError();
    if (status != cudaSuccess) { clean_up(); return status; }
    status = cudaDeviceSynchronize();
    if (status != cudaSuccess) { clean_up(); return status; }

    std::vector<double> partials(blocks);
    status = cudaMemcpy(partials.data(), dev_partials,
                        blocks * sizeof(double), cudaMemcpyDeviceToHost);
    clean_up();
    if (status != cudaSuccess) return status;

    double total = 0.0;
    for (double part : partials) {
        if (!std::isfinite(part)) return cudaErrorInvalidValue;
        total += part;
        if (!std::isfinite(total)) return cudaErrorInvalidValue;
    }
    *out = total;
    return cudaSuccess;
}
