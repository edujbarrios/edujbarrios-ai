# Roadmap

This roadmap is **a plan, not an inventory of implemented features**. The
[category READMEs](algorithms/01_foundations/README.md) and GitHub pull
requests show what has actually shipped. Algorithms are deliberately added
in small, independently reviewable PRs.

## Stage 1 — Numerical foundations

1. Dot product (starting example)
2. Euclidean distance
3. Cosine similarity
4. Vector normalization
5. Matrix multiplication
6. Mean and variance

## Stage 2 — Classical ML

1. K-nearest neighbors
2. Gradient descent
3. Linear regression
4. Logistic regression
5. K-means clustering
6. Principal component analysis

## Stage 3 — Neural network primitives

1. ReLU / sigmoid
2. Stable softmax
3. Cross-entropy
4. Dense forward pass
5. Backpropagation
6. Mini-batch gradient descent

## Stage 4 — Information retrieval

1. Tokenization
2. Term frequency and TF-IDF
3. Inverted index
4. BM25
5. Top-k selection
6. Reciprocal rank fusion

## Stage 5 — Vector search

1. Exact k-nearest-neighbor search
2. Quantization basics
3. Approximate nearest neighbors
4. HNSW indexing
5. Product quantization

## Stage 6 — AI systems

1. Hybrid retrieval
2. Bounded caches
3. Retrieval evaluation metrics
4. Re-ranking primitives
5. Streaming and batching

## Contribution order

Start with small CPU implementations, identical tests, then add optimized
versions or **optional** CUDA kernels when parallelism is actually relevant.
Every algorithm must have a dedicated README, declared inputs/outputs,
edge-case tests and a description of where it fits into AI systems.

**Do not interpret the presence of an algorithm in this list as completion.**
