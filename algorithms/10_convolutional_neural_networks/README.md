# Convolutional Neural Networks (CNNs)

A dedicated section for **convolutional neural networks** and their spatial
building blocks. Reference implementations use small, explicit tensors for
learning and testing the mathematics without deep-learning frameworks.

## Implemented algorithms (4)

| Algorithm | What it does |
| --- | --- |
| [`depthwise_conv2d`](depthwise_conv2d/README.md) | Applies an independent same-padded kernel per feature channel. |
| [`global_average_pool2d`](global_average_pool2d/README.md) | Computes one spatial mean per feature channel. |
| [`max_pool2d`](max_pool2d/README.md) | Reduces feature-map resolution with non-overlapping 2×2 maxima. |
| [`valid_conv2d`](valid_conv2d/README.md) | Applies valid spatial cross-correlation with a 2D kernel. |

[← All sections](../../README.md)
