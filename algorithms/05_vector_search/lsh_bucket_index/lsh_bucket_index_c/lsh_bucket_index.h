#ifndef EDUJBARRIOS_AI_LSH_BUCKET_INDEX_H
#define EDUJBARRIOS_AI_LSH_BUCKET_INDEX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Opaque heap index. Query output has n slots, where n is the build
   corpus length. Caller owns index and must free it. */
typedef struct lsh_bucket_index lsh_bucket_index;
bool lsh_bucket_index_build(const uint32_t *signatures, size_t n,
                            lsh_bucket_index **out_index);
bool lsh_bucket_index_query(const lsh_bucket_index *index, uint32_t signature,
                            size_t *output, size_t *out_count);
void lsh_bucket_index_free(lsh_bucket_index *index);
#endif
