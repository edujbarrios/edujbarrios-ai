# Vision-Language Models (VLMs)

Building blocks for aligning images and text: multimodal similarity, contrastive
learning, image–text retrieval and fusion. Implementations illustrate algorithms
independently of pretrained model weights or large training frameworks.

## Implemented algorithms (9)

| Algorithm | What it does |
| --- | --- |
| [`clip_contrastive_loss`](clip_contrastive_loss/README.md) | Computes stable bidirectional CLIP-style image–text contrastive loss. |
| [`clip_zero_shot_classification`](clip_zero_shot_classification/README.md) | Produces zero-shot class probabilities by matching image and text embeddings. |
| [`cross_modal_attention`](cross_modal_attention/README.md) | Computes text-query attention over image patch keys and values. |
| [`image_text_hard_negatives`](image_text_hard_negatives/README.md) | Finds the closest nonmatching image/text candidate in both retrieval directions. |
| [`image_text_margin_loss`](image_text_margin_loss/README.md) | Ranks matched image–text pairs above hardest negatives using a symmetric margin loss. |
| [`image_text_mrr`](image_text_mrr/README.md) | Evaluates bidirectional retrieval ranking with mean reciprocal rank. |
| [`image_text_recall_at_k`](image_text_recall_at_k/README.md) | Evaluates bidirectional retrieval recall@K for paired image and text embeddings. |
| [`patch_text_maxsim`](patch_text_maxsim/README.md) | Finds each text token's best-matching visual patch and averages cosine scores. |
| [`siglip_pairwise_loss`](siglip_pairwise_loss/README.md) | Computes a stable pairwise sigmoid contrastive loss for image/text pairs. |

[← Repository home](../README.md)
