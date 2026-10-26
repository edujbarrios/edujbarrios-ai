# Classical Machine Learning

This section explores predictive models, clustering, dimensionality reduction and optimization without hiding their mechanics inside an ML framework. Compare supervised methods with unsupervised ones and inspect how their assumptions differ.

## Implemented algorithms (21)

| Algorithm | What it does |
| --- | --- |
| [`dbscan`](dbscan/README.md) | Groups dense neighborhoods into clusters and identifies points treated as noise. |
| [`decision_stump`](decision_stump/README.md) | Fits a single-split decision tree, often used as a weak classifier. |
| [`exponential_moving_average`](exponential_moving_average/README.md) | Smooths sequences with exponentially decaying weights, a standard time-series and optimization primitive. |
| [`farthest_first_traversal`](farthest_first_traversal/README.md) | Selects well-separated representatives by repeatedly choosing the farthest point. |
| [`gaussian_naive_bayes`](gaussian_naive_bayes/README.md) | Classifies continuous features using class-conditional Gaussian probabilities. |
| [`gradient_descent`](gradient_descent/README.md) | Iteratively updates parameters in the direction that reduces a differentiable objective. |
| [`greedy_k_medoids`](greedy_k_medoids/README.md) | Selects representative data points as cluster centers using greedy improvements. |
| [`hmm_forward_algorithm`](hmm_forward_algorithm/README.md) | Computes sequence likelihoods in a hidden Markov model by dynamic programming. |
| [`k_means`](k_means/README.md) | Partitions observations into clusters by updating centroids and assignments. |
| [`knn`](knn/README.md) | Predicts a class from the labels of the nearest training examples. |
| [`knn_regression`](knn_regression/README.md) | Predicts a numerical target from nearby observations. |
| [`linear_regression`](linear_regression/README.md) | Fits a linear relation between inputs and a continuous target. |
| [`logistic_regression`](logistic_regression/README.md) | Models binary class probabilities with a linear score and sigmoid. |
| [`mini_batch_gradient_descent`](mini_batch_gradient_descent/README.md) | Updates model parameters using small subsets of training examples. |
| [`multinomial_naive_bayes`](multinomial_naive_bayes/README.md) | Classifies count-based features with multinomial likelihoods. |
| [`nearest_centroid_classifier`](nearest_centroid_classifier/README.md) | Assigns observations to the class with the closest representative centroid. |
| [`pca_2d`](pca_2d/README.md) | Projects two-dimensional data onto directions of greatest variance. |
| [`perceptron`](perceptron/README.md) | Learns a linear decision boundary from classification mistakes. |
| [`ridge_regression_1d`](ridge_regression_1d/README.md) | Fits one-dimensional linear regression with an L2 penalty on coefficients. |
| [`silhouette_score`](silhouette_score/README.md) | Measures how well points match their own cluster compared with others. |
| [`viterbi_decoding`](viterbi_decoding/README.md) | Finds the most likely hidden-state sequence in a hidden Markov model. |

Each algorithm directory contains a README with its mathematical definition, input contract, limitations and available language implementations. Run its tests using the instructions in the [repository README](../../README.md).

[← All sections](../../README.md)
