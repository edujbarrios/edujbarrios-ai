#include "dot_product.cuh"

#include <cassert>
#include <cmath>
#include <cstdio>

int main() {
    int devices = 0;
    const cudaError_t device_status = cudaGetDeviceCount(&devices);
    if (device_status != cudaSuccess || devices < 1) {
        std::fprintf(stderr, "CUDA test requires an NVIDIA GPU: %s\n",
                     cudaGetErrorString(device_status));
        return 1;
    }

    const double a[] = {1.0, 2.0, 3.0};
    const double b[] = {4.0, 5.0, 6.0};
    const double bad[] = {NAN};
    const double huge[] = {1e308};
    double out = -1.0;

    assert(dot_product_cuda(a, b, 3, &out) == cudaSuccess);
    assert(std::abs(out - 32.0) < 1e-10);
    assert(dot_product_cuda(nullptr, nullptr, 0, &out) == cudaSuccess);
    assert(out == 0.0);
    assert(dot_product_cuda(bad, b, 1, &out) == cudaErrorInvalidValue);
    assert(dot_product_cuda(huge, huge, 1, &out) == cudaErrorInvalidValue);
    assert(out == 0.0);
    std::puts("CUDA dot_product tests passed");
    return 0;
}
