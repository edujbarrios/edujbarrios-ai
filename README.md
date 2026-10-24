# edujbarrios-ai

**An open-source, GitHub-first hub of artificial intelligence algorithms implemented from scratch in C, Python, Rust, R, and (when GPU acceleration makes sense) CUDA.**

From numerical foundations and classical machine learning to neural networks, information retrieval, vector indexes, and AI systems. The goal is to make **each algorithm understandable, testable, reusable, and independently reviewable**—not to create a new AI framework.

## Why this repository exists

- **Learn:** read the explanation, mathematical definition, design choices, and limitations.
- **Use:** take a small, self-contained implementation into a project while respecting its license.
- **Compare:** inspect equivalent behavior across C, Python, Rust, and R; explore CUDA implementations for parallel workloads.
- **Build over time:** contribute **one algorithm (or one focused improvement) per pull request**. This is a living catalogue, not a one-off course.

The emphasis is **correctness and clarity before speed**. Benchmarks, where included, must state their methodology and should not imply that these examples outperform production libraries.

## Languages

| Language | Role | Status |
| --- | --- | --- |
| C | Low-level CPU algorithms and explicit memory handling | Core |
| Python | Readable reference implementations without mandatory ML frameworks | Core |
| Rust | Typed, memory-safe CPU implementations | Core |
| R | Numerical and statistical perspectives | Core |
| CUDA (CUDA C++) | Optional NVIDIA GPU implementations for suitable workloads | Optional per algorithm |

CUDA is based on C++ with NVIDIA extensions; it is **not** a replacement for C, and GPU hardware is not required to use the CPU examples.

## Explore the catalogue

| Stage | Area | What you will find |
| --- | --- | --- |
| [01](algorithms/01_foundations/README.md) | Foundations | Vector operations, distances, normalization |
| [02](algorithms/02_classical_ml/README.md) | Classical ML | KNN, regression, optimization, clustering |
| [03](algorithms/03_neural_networks/README.md) | Neural Networks | Activations, losses, backpropagation |
| [04](algorithms/04_information_retrieval/README.md) | Information Retrieval | Tokenization, TF-IDF, inverted indexes, BM25 |
| [05](algorithms/05_vector_search/README.md) | Vector Search | Exact nearest neighbors, ANN, HNSW |
| [06](algorithms/06_ai_systems/README.md) | AI Systems | Hybrid retrieval, ranking fusion, systems primitives |

The merged algorithm directories are the source of truth for the catalogue. Each implementation has its own documentation and tests.

## Repository conventions

```text
algorithms/
  01_foundations/
    dot_product/
      README.md                 # why it belongs here, math, behavior, how to run
      algorithm.json            # machine-readable metadata
      dot_product_c/            # C source + tests
      dot_product_python/       # Python source + tests
      dot_product_rust/         # Rust source + tests
      dot_product_r/            # R source + tests
      dot_product_cuda/         # optional CUDA source + tests
```

**Naming rule:** inside each algorithm, implementation directories are named `<algorithm_slug>_<language>` (snake_case), e.g. `cosine_similarity_rust` or `bm25_python`. Every algorithm has its **own README**, even if it only has one implementation so far.

Each algorithm must describe accepted inputs, edge cases, complexity, known limitations, usage, tests and any sources. Implementations in different languages follow the same documented contract; differences must be explained.

## Test the repository

The tooling uses the Python standard library. To run CPU checks locally, install Python 3, a C compiler, Rust, and R:

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
# Run only one language, matching the parallel CI jobs:
python3 scripts/run_tests.py --strict --language python
```

CUDA checks are **opt-in**, require `nvcc` and a compatible NVIDIA GPU, and are never silently represented as CPU-tested:

```bash
python3 scripts/run_tests.py --strict --with-cuda
```

For prerequisites and contribution rules see [CONTRIBUTING.md](CONTRIBUTING.md) and [docs/ALGORITHM_STANDARD.md](docs/ALGORITHM_STANDARD.md).

## Open source and contributions

This project welcomes issues, corrections and focused pull requests. Add algorithms **incrementally**; avoid bulk-generated submissions or translation-only code without tests. Prefer a reproducible, small example with honest limitations over a feature-rich but unverified implementation.

Licensed under **Apache License 2.0**. See [LICENSE](LICENSE) and [NOTICE](NOTICE). Individual contributions must be original or legally compatible and properly attributed.

Maintained by [@edujbarrios](https://github.com/edujbarrios).
