# Vision-Language Models (VLMs)

Building blocks for aligning images and text: multimodal similarity, contrastive
learning, image–text retrieval and fusion. Implementations illustrate algorithms
independently of pretrained model weights or large training frameworks.

## Implemented algorithms (5)

| Algorithm | What it does |
| --- | --- |
| [`clip_contrastive_loss`](clip_contrastive_loss/README.md) | Computes stable bidirectional CLIP-style image–text contrastive loss. |
| [`clip_zero_shot_classification`](clip_zero_shot_classification/README.md) | Produces zero-shot class probabilities by matching image and text embeddings. |
| [`image_text_hard_negatives`](image_text_hard_negatives/README.md) | Finds the closest nonmatching image/text candidate in both retrieval directions. |
| [`image_text_recall_at_k`](image_text_recall_at_k/README.md) | Evaluates bidirectional retrieval recall@K for paired image and text embeddings. |

| [`siglip_pairwise_loss`](siglip_pairwise_loss/README.md) | Computes a stable pairwise sigmoid contrastive loss for image/text pairs. |

[← Repository home](../README.md)
