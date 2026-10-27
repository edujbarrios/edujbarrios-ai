# Bidirectional Image–Text Mean Reciprocal Rank

## Purpose

Mean reciprocal rank (MRR) evaluates **how early** a query's first
relevant result appears in a ranked result list. For image–text
retrieval, image embeddings can query text embeddings and vice versa.
This reference assumes exactly one matching image/text pair per
index and computes cosine-based rankings (Voorhees, 1999).

## Algorithm

For `N` image rows and `N` text rows of dimension `D`:

1. Normalize each row to unit L2 length.
2. Compute `S[i,j] = dot(image[i], text[j])`.
3. For each image query `i`, rank the correct text `j=i` among
   candidates in row `i` by decreasing score. For text queries,
   rank image `i` among candidates in column `i`.
4. For equal scores, the **lower zero-based candidate index**
   ranks earlier. The rank is 1-based.
5. Return separate means `sum(1/rank) / N` for the image-to-text
   and text-to-image retrieval directions.

Example with 2 orthogonal matching rows in both modalities:
`image_to_text_MRR=1` and `text_to_image_MRR=1`.
Swapping the text embeddings produces `[0.5,0.5]`.
Two identical rows in both modalities result in `[0.75,0.75]`
because the correct pair ranks first for index 0, second for index 1.

## Contract

- `N=2..64` matching rows, `D=1..128` dimensions.
- Images and texts are flat **row-major** numeric arrays of
  exactly `N*D` values; finite values must lie in
  `[-1000,1000]`. Every row has a strictly positive norm.
- Output is two real numbers in `(0,1]`, ordered
  `[image_to_text_mrr, text_to_image_mrr]`.
- Invalid dimensions, input lengths, nonfinite values or zero
  vectors are rejected. Python rejects booleans as numbers.
- C writes two doubles into a separate caller-owned output array,
  unchanged on error. Rust returns `Result<[f64;2],&str>`;
  Python returns a two-tuple and R a two-element numeric vector.

## Complexity

Constructing the normalized vectors and similarity matrix takes
**O(N²D)** time and **O(ND+N²)** auxiliary memory. Counting the
number of candidates outranking each diagonal match is O(N²).
No library-based ranking or approximate search is used.

## Implementations

| Language | Source and tests |
| --- | --- |
| C | [image_text_mrr_c](image_text_mrr_c/) |
| Python | [image_text_mrr_python](image_text_mrr_python/) |
| Rust | [image_text_mrr_rust](image_text_mrr_rust/) |
| R | [image_text_mrr_r](image_text_mrr_r/) |

Run `python3 scripts/validate_repository.py` and
`python3 scripts/run_tests.py --strict`.

Tests cover perfect alignment, reversed pairs, all-equal ties,
scale invariance, invalid row lengths and zero/nonfinite embeddings.

## Limitations

This variant assumes exactly one correct partner per query.
It is not appropriate for datasets with multiple equally valid
captions per image unless relevance annotations are extended.
The module does not train encoders or return ranked candidate lists.

## References

- Voorhees, E. M. (1999). *The TREC-8 question answering track report*. Proceedings of the Eighth Text Retrieval Conference (TREC-8). National Institute of Standards and Technology. https://trec.nist.gov/pubs/trec8/papers/qa_report.pdf
