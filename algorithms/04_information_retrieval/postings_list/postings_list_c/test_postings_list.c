#include "postings_list.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const int32_t tokens[] = {1,2,1,2,1,3};
    const size_t offsets[] = {0,3,3,5,6};
    const size_t bad_offsets[] = {0,3,2,5,6};
    const int32_t negative[] = {-5,-5};
    const size_t negative_offsets[] = {0,2};
    const size_t empty_offsets[] = {0};
    size_t ids[] = {99,99,99,99};
    size_t frequencies[] = {99,99,99,99};
    size_t count = 99;

    assert(postings_list(tokens,6,offsets,4,1,ids,frequencies,&count));
    assert(count == 2 && ids[0]==0 && frequencies[0]==2 &&
           ids[1]==2 && frequencies[1]==1);
    assert(postings_list(tokens,6,offsets,4,4,ids,frequencies,&count));
    assert(count == 0);
    assert(postings_list(NULL,0,empty_offsets,0,-5,NULL,NULL,&count));
    assert(count == 0);
    assert(postings_list(negative,2,negative_offsets,1,-5,ids,frequencies,&count));
    assert(count==1 && ids[0]==0 && frequencies[0]==2);

    count = 99; ids[0] = 99; frequencies[0] = 99;
    assert(!postings_list(tokens,6,bad_offsets,4,1,ids,frequencies,&count));
    assert(!postings_list(NULL,6,offsets,4,1,ids,frequencies,&count));
    assert(!postings_list(tokens,6,offsets,4,1,ids,frequencies,NULL));
    assert(count == 99 && ids[0] == 99 && frequencies[0] == 99);
    puts("C postings list tests passed");
    return 0;
}
