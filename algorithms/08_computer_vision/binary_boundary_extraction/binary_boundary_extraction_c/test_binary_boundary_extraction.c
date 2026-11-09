#include "binary_boundary_extraction.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const uint8_t *data,size_t h,size_t w,const uint8_t *want) {
    uint8_t output[4096]={0};
    assert(binary_boundary_extraction(data,h,w,output));
    for(size_t i=0;i<h*w;++i) assert(output[i]==want[i]);
}
int main(void){
    const uint8_t full[]={1,1,1,1,1,1,1,1,1};
    const uint8_t full_edge[]={1,1,1,1,0,1,1,1,1};
    const uint8_t center[]={0,0,0,0,1,0,0,0,0};
    const uint8_t ring[]={1,1,1,1,0,1,1,1,1};
    const uint8_t zeros[]={0,0,0,0,0,0,0,0,0};
    check(full,3,3,full_edge);
    check(center,3,3,center);
    check(ring,3,3,ring);
    check(zeros,3,3,zeros);
    const uint8_t one[]={1},zero[]={0};
    check(one,1,1,one);check(zero,1,1,zero);
    const uint8_t line[]={1,1,1,1};
    check(line,1,4,line);check(line,4,1,line);
    const uint8_t diag[]={1,0,0,1};
    check(diag,2,2,diag);
    uint8_t big[4096],output[4096];
    memset(big,1,sizeof(big));
    assert(binary_boundary_extraction(big,64,64,output));
    size_t sum=0;
    for(size_t i=0;i<4096;++i) sum+=output[i];
    assert(sum==252 && output[0]==1 && output[32*64+32]==0);
    memset(big,0,sizeof(big));
    assert(binary_boundary_extraction(big,64,64,output));
    for(size_t i=0;i<4096;++i)assert(output[i]==0);
    output[0]=77;
    const uint8_t invalid[]={0,2};
    assert(!binary_boundary_extraction(invalid,1,2,output));
    assert(!binary_boundary_extraction(big,0,1,output));
    assert(!binary_boundary_extraction(big,1,0,output));
    assert(!binary_boundary_extraction(big,65,1,output));
    assert(!binary_boundary_extraction(big,1,65,output));
    assert(!binary_boundary_extraction(NULL,1,1,output));
    assert(!binary_boundary_extraction(big,1,1,NULL));
    assert(output[0]==77);
    uint8_t inplace[]={1};
    assert(!binary_boundary_extraction(inplace,1,1,inplace));
    assert(inplace[0]==1);
    puts("C binary boundary extraction tests passed");
    return 0;
}
