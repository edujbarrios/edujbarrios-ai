# Foundations

These algorithms provide the numerical building blocks used in machine learning, embeddings, probability and data preprocessing. Start with vector operations, then distances and normalization; move to probability and stable numerical computations.

## Implemented algorithms (21)

| Algorithm | What it does |
| --- | --- |
| [`bray_curtis_dissimilarity`](bray_curtis_dissimilarity/README.md) | Compares nonnegative feature profiles with relative dissimilarity. |
| [`canberra_distance`](canberra_distance/README.md) | Measures relative coordinate differences with zero-safe scaling. |
| [`chebyshev_distance`](chebyshev_distance/README.md) | Measures the largest absolute coordinate difference (L∞ distance) between two vectors. |
| [`cosine_similarity`](cosine_similarity/README.md) | Measures the angle-based similarity between two vectors, independent of their magnitudes. |
| [`dot_product`](dot_product/README.md) | Multiplies corresponding vector elements and sums the products; a basis for linear models and attention. |
| [`euclidean_distance`](euclidean_distance/README.md) | Computes straight-line (L2) distance between points in a vector space. |
| [`gaussian_rbf_kernel`](gaussian_rbf_kernel/README.md) | Transforms squared Euclidean distance into a smooth similarity through a Gaussian kernel. |
| [`hellinger_distance`](hellinger_distance/README.md) | Computes bounded symmetric distance between discrete probability distributions. |
| [`jensen_shannon_divergence`](jensen_shannon_divergence/README.md) | Measures symmetric dissimilarity between probability distributions using their mixture. |
| [`kullback_leibler_divergence`](kullback_leibler_divergence/README.md) | Quantifies how one probability distribution differs from a reference distribution. |
| [`logsumexp`](logsumexp/README.md) | Computes the logarithm of a sum of exponentials while reducing overflow and underflow. |
| [`manhattan_distance`](manhattan_distance/README.md) | Computes L1 distance by summing absolute coordinate differences between vectors. |
| [`matrix_multiplication`](matrix_multiplication/README.md) | Combines matrices through row-by-column products, fundamental to dense layers. |
| [`mean_variance`](mean_variance/README.md) | Calculates central tendency and spread for numerical observations. |
| [`min_max_scaling`](min_max_scaling/README.md) | Rescales features relative to their observed minimum and maximum. |
| [`minkowski_distance`](minkowski_distance/README.md) | Computes L-p distance for any finite p at least one. |
| [`pearson_correlation`](pearson_correlation/README.md) | Measures linear association between two paired numerical features. |
| [`shannon_entropy`](shannon_entropy/README.md) | Quantifies the uncertainty or information content of a probability distribution. |
| [`total_variation_distance`](total_variation_distance/README.md) | Measures bounded differences between discrete probability distributions. |
| [`vector_normalization`](vector_normalization/README.md) | Scales a vector to unit length for magnitude-independent comparisons. |
| [`z_score_standardization`](z_score_standardization/README.md) | Centers and scales values using their mean and standard deviation. |

Each algorithm directory contains a README with its mathematical definition, input contract, limitations and available language implementations. Run its tests using the instructions in the [repository README](../../README.md).

[← All sections](../../README.md)
