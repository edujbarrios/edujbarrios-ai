#include "postings_difference.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
int main(void) {
    const uint32_t a[]={0,2,4,7};
    const uint32_t b[]={1,2,5,7};
    const uint32_t bad[]={1,1};
    const uint32_t descending[]={3,2};
    const uint32_t out_of_range[]={UINT32_MAX};
    const uint32_t max[]={INT32_MAX};
    uint32_t out[]={99,99,99,99};
    size_t count=99;
    assert(postings_difference(a,4,b,4,out,&count));
    assert(count==2 && out[0]==0 && out[1]==4);
    assert(postings_difference(a,4,a,4,out,&count));
    assert(count==0);
    assert(postings_difference(a,4,NULL,0,out,&count));
    assert(count==4 && out[0]==0 && out[3]==7);
    assert(postings_difference(NULL,0,a,4,NULL,&count) && count==0);
    assert(postings_difference(NULL,0,NULL,0,NULL,&count) && count==0);
    assert(postings_difference(max,1,NULL,0,out,&count));
    assert(count==1 && out[0]==INT32_MAX);
    count=99;out[0]=99;
    assert(!postings_difference(a,4,bad,2,out,&count));
    assert(!postings_difference(NULL,0,descending,2,NULL,&count));
    assert(!postings_difference(out_of_range,1,NULL,0,out,&count));
    assert(!postings_difference(a,4,b,4,NULL,&count));
    assert(!postings_difference(a,4,b,4,out,NULL));
    assert(count==99 && out[0]==99);
    puts("C postings difference tests passed");
    return 0;
}
