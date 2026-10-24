#include "lsh_bucket_index.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const uint32_t signatures[]={3,7,3,0,7};
    const uint32_t invalid[]={3,UINT32_MAX};
    const uint32_t bound[]={(UINT32_C(1)<<30)-1};
    lsh_bucket_index *index=NULL;
    size_t positions[]={99,99,99,99,99},count=99;
    assert(lsh_bucket_index_build(signatures,5,&index));
    assert(index!=NULL);
    assert(lsh_bucket_index_query(index,3,positions,&count));
    assert(count==2 && positions[0]==0 && positions[1]==2);
    assert(lsh_bucket_index_query(index,7,positions,&count));
    assert(count==2 && positions[0]==1 && positions[1]==4);
    assert(lsh_bucket_index_query(index,4,positions,&count) && count==0);
    assert(lsh_bucket_index_query(index,3,positions,&count) && count==2);
    count=99;positions[0]=99;
    assert(!lsh_bucket_index_query(index,UINT32_MAX,positions,&count));
    assert(!lsh_bucket_index_query(index,3,NULL,&count));
    assert(!lsh_bucket_index_query(NULL,3,positions,&count));
    assert(count==99 && positions[0]==99);
    lsh_bucket_index *other=index;
    assert(!lsh_bucket_index_build(invalid,2,&other));
    assert(other==index);
    assert(!lsh_bucket_index_build(signatures,0,&other));
    lsh_bucket_index_free(index);
    index=NULL;
    assert(lsh_bucket_index_build(bound,1,&index));
    assert(lsh_bucket_index_query(index,(UINT32_C(1)<<30)-1,positions,&count));
    assert(count==1 && positions[0]==0);
    lsh_bucket_index_free(index);
    lsh_bucket_index_free(NULL);
    puts("C LSH bucket index tests passed");
    return 0;
}
