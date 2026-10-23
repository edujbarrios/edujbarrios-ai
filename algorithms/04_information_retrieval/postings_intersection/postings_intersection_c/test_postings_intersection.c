#include "postings_intersection.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const uint32_t a[] = {0,2,4,9};
    const uint32_t b[] = {1,2,4,8};
    const uint32_t dup[] = {1,1,2};
    const uint32_t unordered[] = {5,2};
    const uint32_t huge[] = {UINT32_MAX};
    uint32_t out[] = {99,99,99,99};
    size_t count = 99;

    assert(postings_intersection(a,4,b,4,out,&count));
    assert(count==2 && out[0]==2 && out[1]==4);
    assert(postings_intersection(a,4,a,4,out,&count));
    assert(count==4 && out[0]==0 && out[3]==9);
    assert(postings_intersection(NULL,0,a,4,NULL,&count) && count==0);

    out[0]=99; count=99;
    assert(!postings_intersection(dup,3,a,4,out,&count));
    assert(!postings_intersection(unordered,2,a,4,out,&count));
    assert(!postings_intersection(huge,1,a,4,out,&count));
    assert(!postings_intersection(NULL,2,a,4,out,&count));
    assert(!postings_intersection(a,4,b,4,NULL,&count));
    assert(!postings_intersection(a,4,b,4,out,NULL));
    assert(out[0]==99 && count==99);
    puts("C postings intersection tests passed");
    return 0;
}
