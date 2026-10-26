# Information Retrieval

Algorithms for lexical retrieval, ranking, combining results, graph traversal and text similarity. These are useful when studying search engines, recommender systems and retrieval-augmented generation (RAG).

## Implemented algorithms (19)

| Algorithm | What it does |
| --- | --- |
| [`astar_shortest_path`](astar_shortest_path/README.md) | Finds low-cost paths using known costs plus a heuristic estimate. |
| [`bm25`](bm25/README.md) | Ranks documents against queries using term frequency, document length and inverse document frequency. |
| [`dijkstra_shortest_path`](dijkstra_shortest_path/README.md) | Computes minimum-cost paths from a source in a nonnegative-weight graph. |
| [`floyd_warshall`](floyd_warshall/README.md) | Computes all-pairs shortest paths using dynamic programming. |
| [`hits_hubs_authorities`](hits_hubs_authorities/README.md) | Iteratively estimates hub and authority scores on a link graph. |
| [`jaccard_similarity`](jaccard_similarity/README.md) | Measures overlap between sets relative to their union. |
| [`kruskal_mst`](kruskal_mst/README.md) | Constructs a minimum spanning tree by greedily selecting safe edges. |
| [`levenshtein_distance`](levenshtein_distance/README.md) | Counts the minimum edits needed to turn one string into another. |
| [`maximal_marginal_relevance`](maximal_marginal_relevance/README.md) | Balances relevance with diversity when reranking search candidates. |
| [`pagerank`](pagerank/README.md) | Estimates node importance through iterative link-based probability propagation. |
| [`positional_phrase_search`](positional_phrase_search/README.md) | Matches consecutive terms by intersecting positional posting lists. |
| [`postings_difference`](postings_difference/README.md) | Returns document identifiers present in one postings list but not another. |
| [`postings_intersection`](postings_intersection/README.md) | Finds documents shared by two sorted postings lists. |
| [`postings_list`](postings_list/README.md) | Represents term-to-document matches in a searchable posting structure. |
| [`postings_union`](postings_union/README.md) | Combines sorted postings lists without duplicate document identifiers. |
| [`reciprocal_rank_fusion`](reciprocal_rank_fusion/README.md) | Combines ranked result lists without requiring comparable relevance scores. |
| [`rocchio_relevance_feedback`](rocchio_relevance_feedback/README.md) | Updates a query vector using relevant and nonrelevant document vectors. |
| [`tf_idf`](tf_idf/README.md) | Weights document terms by local frequency and global rarity. |
| [`top_k`](top_k/README.md) | Selects the highest-ranked items from a collection of scores. |

Each algorithm directory contains a README with its mathematical definition, input contract, limitations and available language implementations. Run its tests using the instructions in the [repository README](../../README.md).

[← All sections](../../README.md)
