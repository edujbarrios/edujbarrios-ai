# Vector Search

This category compares exact and approximate similarity search over embeddings and compact signatures. Study distance scoring first, then indexing and quantization to understand latency–quality trade-offs.

## Implemented algorithms (8)

| Algorithm | What it does |
| --- | --- |
| [`euclidean_top_k`](euclidean_top_k/README.md) | Retrieves the nearest candidates according to L2 distance. |
| [`exact_cosine_top_k`](exact_cosine_top_k/README.md) | Ranks vector candidates by their exact cosine similarity to a query. |
| [`hamming_radius_search`](hamming_radius_search/README.md) | Finds binary signatures within a specified Hamming distance. |
| [`hamming_top_k`](hamming_top_k/README.md) | Returns binary signatures with the smallest number of differing bits. |
| [`lsh_bucket_index`](lsh_bucket_index/README.md) | Groups similar signatures into locality-sensitive hash buckets for candidate retrieval. |
| [`maximum_inner_product_top_k`](maximum_inner_product_top_k/README.md) | Retrieves vectors with the highest query–candidate dot products. |
| [`random_hyperplane_hash`](random_hyperplane_hash/README.md) | Maps vectors to sign-based binary signatures using random hyperplanes. |
| [`scalar_quantization`](scalar_quantization/README.md) | Compresses continuous values into discrete quantization levels. |

Each algorithm directory contains a README with its mathematical definition, input contract, limitations and available language implementations. Run its tests using the instructions in the [repository README](../../README.md).

[← All sections](../../README.md)
