#include "postings_union.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
int main(void) {
    const uint32_t a[]={0,2,4};
    const uint32_t b[]={1,2,5};
    const uint32_t dup[]={1,1};
    const uint32_t descending[]={5,4};
    const uint32_t large[]={UINT32_MAX};
    const uint32_t boundary[]={(uint32_t)INT32_MAX};
    uint32_t output[]={99,99,99,99,99,99};
    size_t count=99;
    assert(postings_union(a,3,b,3,output,&count));
    assert(count==5 && output[0]==0 && output[1]==1 &&
           output[2]==2 && output[3]==4 && output[4]==5);
    assert(postings_union(NULL,0,a,3,output,&count));
    assert(count==3 && output[0]==0 && output[2]==4);
    assert(postings_union(a,3,NULL,0,output,&count));
    assert(count==3 && output[0]==0 && output[2]==4);
    assert(postings_union(NULL,0,NULL,0,NULL,&count) && count==0);
    assert(postings_union(boundary,1,NULL,0,output,&count));
    assert(count==1 && output[0]==(uint32_t)INT32_MAX);
    count=99;output[0]=99;
    assert(!postings_union(a,3,dup,2,output,&count));
    assert(!postings_union(NULL,0,descending,2,output,&count));
    assert(!postings_union(large,1,b,3,output,&count));
    assert(!postings_union(a,3,b,3,NULL,&count));
    assert(!postings_union(a,3,b,3,output,NULL));
    assert(count==99 && output[0]==99);
    puts("C postings union tests passed");
    return 0;
}
