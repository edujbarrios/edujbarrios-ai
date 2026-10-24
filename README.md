# edujbarrios-ai

**AI algorithms, from scratch and open to everyone.**

**Author:** Eduardo J. Barrios · **Email:** [edujbarrios@outlook.com](mailto:edujbarrios@outlook.com)

Given my background in software engineering and artificial intelligence, I started this project with a simple goal: **make the core algorithms behind AI accessible to everyone**.

By implementing them from scratch in **C, Python, Rust, and R**, I want to make it easier to understand how they work, explore the mathematics behind them, and learn through real, tested code.

This is an **open-source, growing catalogue** of AI algorithms and building blocks—from numerical foundations and classical machine learning to neural networks, information retrieval, vector search, and AI evaluation. **New algorithms are added over time, one focused contribution at a time.** CUDA implementations may be included when GPU acceleration is appropriate and can be tested on real hardware.

The aim isn't to replace mature libraries or claim production-level speed. It is to **make the ideas underneath them transparent**, with understandable explanations, explicit mathematical and API contracts, examples, citations, and tests.

## Why this repository exists

- **Understand the fundamentals.** Read the mathematics, implementation choices, and trade-offs behind each algorithm.
- **Learn by doing.** Explore small, self-contained implementations and run their tests without depending on an AI framework.
- **Compare languages.** See the same algorithm expressed in C, Python, Rust, and R under a shared documented contract.
- **Keep building together.** The catalogue evolves through small, reviewable pull requests, with correctness and clarity ahead of benchmarks or hype.

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

| Category | Area | Examples of what's implemented |
| --- | --- | --- |
| [01](algorithms/01_foundations/README.md) | Foundations | Vector mathematics, distances, normalization, feature scaling |
| [02](algorithms/02_classical_ml/README.md) | Classical ML | Regression, classification, clustering, optimization |
| [03](algorithms/03_neural_networks/README.md) | Neural Networks | Activations, loss functions, backpropagation, attention |
| [04](algorithms/04_information_retrieval/README.md) | Information Retrieval | TF-IDF, BM25, postings operations, edit distance |
| [05](algorithms/05_vector_search/README.md) | Vector Search | Exact cosine search, LSH signatures, quantization |
| [06](algorithms/06_ai_systems/README.md) | AI Systems | Ranking and classification evaluation, score fusion |

The merged algorithm directories are the **source of truth** for what is available today; new implementations are added as they are reviewed and tested.

## Start with an algorithm

Each entry has its own explanation, mathematical definition, language-specific implementation, and tests. Here are a few ways in:

| If you want to explore... | Start here |
| --- | --- |
| Vector similarity | [Cosine Similarity](algorithms/01_foundations/cosine_similarity/) |
| A classical classifier | [Logistic Regression](algorithms/02_classical_ml/logistic_regression/) |
| Transformer building blocks | [Scaled Dot-Product Attention](algorithms/03_neural_networks/scaled_dot_product_attention/) |
| Search relevance | [BM25](algorithms/04_information_retrieval/bm25/) |
| Embedding retrieval | [Exact Cosine Top-K](algorithms/05_vector_search/exact_cosine_top_k/) |
| Ranking evaluation | [NDCG@K](algorithms/06_ai_systems/ndcg_at_k/) |

Pick an algorithm, read its README, and run its implementation in the language you prefer. No framework setup is needed for the core reference implementations.

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

## Author and contact

Created and maintained by **Eduardo J. Barrios** ([@edujbarrios](https://github.com/edujbarrios)).

**Email:** [edujbarrios@outlook.com](mailto:edujbarrios@outlook.com). For code changes, please use GitHub issues and pull requests where possible so discussions remain public and reproducible.
