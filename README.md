# edujbarrios-ai

**AI algorithms from scratch, with explanations and tests.**

A growing educational catalogue implemented in **C, Python, Rust and R** (optional CUDA when suitable). Each algorithm has its own README, documented behavior, source code and tests. The examples favor clarity and correctness over production-level performance.

## Explore

| Section | Focus |
| --- | --- |
| [01 · Foundations](algorithms/01_foundations/README.md) | Vector math, probability and numerical methods |
| [02 · Classical ML](algorithms/02_classical_ml/README.md) | Regression, clustering, classification and optimization |
| [03 · Neural Networks](algorithms/03_neural_networks/README.md) | Activations, losses, gradients and attention |
| [04 · Information Retrieval](algorithms/04_information_retrieval/README.md) | Search, ranking, postings and graphs |
| [05 · Vector Search](algorithms/05_vector_search/README.md) | Nearest neighbors, hashing and quantization |
| [06 · AI Systems](algorithms/06_ai_systems/README.md) | Evaluation metrics and generation utilities |
| [08 · Computer Vision](algorithms/08_computer_vision/README.md) | Image processing, detections and visual representations |
| [10 · Convolutional Neural Networks (CNNs)](algorithms/10_convolutional_neural_networks/README.md) | Convolutions, pooling and spatial feature extraction |

## LLMs — Large Language Models

Explore the standalone [LLM algorithms collection](llms/README.md), with
autoregressive decoding, token filtering and generation utilities.

## VLMs — Vision-Language Models

Explore the standalone [VLM algorithms collection](vlms/README.md), focused
on multimodal contrastive objectives, image–text matching and retrieval.

## Validate and test

```bash
python3 scripts/validate_repository.py
python3 scripts/run_tests.py --strict
```

CPU checks require the corresponding installed toolchains (C, Python, Rust and R). Run a subset with `--language python` (or `c`, `rust`, `r`); CUDA is opt-in via `--with-cuda` and requires NVIDIA hardware.

See [contribution guidelines](CONTRIBUTING.md) and the [algorithm standard](docs/ALGORITHM_STANDARD.md) for conventions and licensing requirements.

**Author:** Eduardo J. Barrios · [GitHub](https://github.com/edujbarrios) · [Email](mailto:edujbarrios@outlook.com)  
**License:** [Apache 2.0](LICENSE) · [NOTICE](NOTICE)
