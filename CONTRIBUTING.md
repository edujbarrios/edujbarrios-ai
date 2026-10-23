# Contributing to edujbarrios-ai

Thank you for contributing! This is an open-source collection of **small,
correct, explained implementations**, not a framework or copy-paste dump.

## Contribution model

- **One algorithm per pull request**, or one focused bug fix, optimization,
  test improvement or additional language implementation.
- Open an issue for large or cross-cutting work before implementation.
- Keep changes independent; no unrelated reformatting.
- Use English for code comments, documentation, PR descriptions and issues.
- Use **APA 7th edition references** in every algorithm README, with verified bibliographic metadata and author–date citations where appropriate.
- Submit original code, or code whose reuse is legally compatible with
  Apache-2.0, with explicit attribution when required.
- Do not claim production readiness, novelty or benchmark wins without proof.

## How to add an algorithm

1. Choose a category from `algorithms/`, and a `snake_case` slug such as
   `cosine_similarity`.
2. Create `algorithms/<category>/<slug>/README.md` using
   [the template](templates/ALGORITHM_README.md). Explain **why it belongs
   here**, what it does, its mathematical contract, limitations and examples.
3. Add `algorithm.json` with a matching `slug`, `title`, `level`,
   `summary` and implemented `languages`.
4. Inside that algorithm, use directories named **`<slug>_<language>`**:
   `dot_product_c`, `dot_product_python`, `dot_product_rust`,
   `dot_product_r`, and (if suitable) `dot_product_cuda`.
5. Name the source file `<slug>.<extension>` and its test
   `test_<slug>.<extension>` (Rust tests can be embedded in the source).
6. Include reproducible tests for empty input, boundary conditions and
   invalid input, where applicable. Use equivalent test cases across languages.
7. Run the validation and CPU test commands below, and report results in the PR.
8. Open a PR describing the problem, the contract, the sources consulted,
   tests executed and untested environments.

CPU languages (C, Python, Rust, R) are the recommended reference set.
A CUDA implementation is **optional per algorithm**, because GPU acceleration
does not always improve performance. Partial algorithm contributions are
acceptable when the missing languages are clearly declared in `algorithm.json`
and the README—never mark an unimplemented language as supported.

## Local checks

Requires Python 3.10+ for repository tooling, plus GCC/Clang, Rust and R
for full CPU coverage:

    python3 scripts/validate_repository.py
    python3 scripts/run_tests.py --strict

Optional GPU checks require `nvcc` and a compatible NVIDIA GPU:

    python3 scripts/run_tests.py --strict --with-cuda

CI runs structural validation and the CPU tests. It does **not** verify CUDA
unless a GPU runner is explicitly configured; PRs with CUDA code must report
manual GPU test evidence.

## Review quality bar

- [ ] Algorithm README explains purpose, math/logic, assumptions and complexity.
- [ ] Folder names follow `<slug>_<language>`.
- [ ] Implementation is standalone, predictable and handles invalid input.
- [ ] Cross-language semantics match or justified differences are documented.
- [ ] Tests are deterministic, locally reproducible and actually pass.
- [ ] Performance claims include machine/configuration, inputs and methodology.
- [ ] References use APA 7 format, with verified authors/dates/titles and stable links; licensing obligations are recorded separately.

See [Algorithm Standard](docs/ALGORITHM_STANDARD.md) for the complete contract.
