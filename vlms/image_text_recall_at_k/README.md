# Bidirectional Image–Text Retrieval Recall at K

## Use in VLMs

Vision–language models are often evaluated on how well an image
retrieves its matching text, and vice versa. Recall@K measures the
fraction of queries with the correct paired result among their first
K candidates (Radford et al., 2021). This reference evaluates a
small batch of **one-to-one, position-matched pairs** using cosine
similarity and reproducible tie-breaking.

## Algorithm

Given `N` image embeddings `I[i]` and `N` text embeddings `T[j]`
of dimension `D`, normalize every embedding to length 1, then form

```text
similarity[i,j] = dot(I[i]/||I[i]||, T[j]/||T[j]||)
```

For each image query `i`, the correct text is `j=i`.
Its rank is the number of candidate texts whose similarity is larger,
**plus** any equally scored candidate with a *smaller zero-based index*.
The text-to-image direction uses each **column** and the same tie rule.
An item counts as successfully retrieved when its rank is less than K.

Return `[image_to_text_recall, text_to_image_recall]`, each a
fraction in `[0,1]`.

With identity embeddings `[[1,0],[0,1]]` for both modalities,
recall@1 is `[1,1]`. Swapping the text rows yields `[0,0]`.
When every row has the same embedding, recall@1 is `[0.5,0.5]`
for two pairs (first-index tie policy), and recall@2 is `[1,1]`.

## Contract

- `2<=N<=64`, `1<=D<=128`, `1<=K<=N`.
- Both inputs are flat **row-major** arrays of length `N*D`;
  each entry is finite and in `[-1000,1000]`.
- Every image and text row must have nonzero L2 norm.
  Duplicate rows are allowed and resolved by the index tie rule.
- Invalid shapes, nonfinite values, zero norm, and invalid K are
  rejected; Python explicitly rejects boolean IDs/dimensions.
- C writes two outputs in the order above to a separate caller-owned
  length-two array, which remains unchanged if validation fails;
  output must not alias input. Python returns a pair of floats,
  Rust returns `Result<[f64;2],&str>`, and R returns a length-two
  numeric vector. All outputs are bounded fractions.

## Complexity

Building L2-normalized rows and the full similarity matrix requires
**O(N²D)** time and **O(ND + N²)** extra space. Ranking each diagonal
match takes O(N²) additional time. The reference does not use
approximate nearest neighbors or external dependencies.

## Implementations

| Language | Directory |
| --- | --- |
| C | [image_text_recall_at_k_c](image_text_recall_at_k_c/) |
| Python | [image_text_recall_at_k_python](image_text_recall_at_k_python/) |
| Rust | [image_text_recall_at_k_rust](image_text_recall_at_k_rust/) |
| R | [image_text_recall_at_k_r](image_text_recall_at_k_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover aligned/swapped pairs, degenerate tied embeddings,
K=N, scale invariance, and malformed/zero/nonfinite inputs.

## Limitations

This **paired diagonal ground-truth** formulation assumes exactly
one relevant item per query and the same batch size in each modality.
It is unsuitable for datasets with several valid captions per image
without modifying the relevance labels. Recall@K is not a calibrated
classification confidence, and this module does not train or
encode images or language.

## References

- Radford, A., Kim, J. W., Hallacy, C., Ramesh, A., Goh, G., Agarwal, S., Sastry, G., Askell, A., Mishkin, P., Clark, J., Krueger, G., & Sutskever, I. (2021). *Learning transferable visual models from natural language supervision*. Proceedings of Machine Learning Research, 139, 8748–8763. https://proceedings.mlr.press/v139/radford21a.html
