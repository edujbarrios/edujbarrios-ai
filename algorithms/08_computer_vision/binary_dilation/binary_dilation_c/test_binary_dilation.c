#include "binary_dilation.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint8_t center[]={0,0,0,0,1,0,0,0,0};
    const uint8_t corner[]={1,0,0,0,0,0,0,0,0};
    const uint8_t expected[]={1,1,0,1,1,0,0,0,0};
    const uint8_t row[]={0,1,0}, zero[]={0}, one[]={1};
    const uint8_t invalid[]={0,0,0,0,2,0,0,0,0};
    uint8_t out[9]={99,99,99,99,99,99,99,99,99};
    assert(binary_dilation(center,3,3,out));
    for(size_t i=0;i<9;++i) assert(out[i]==1);
    assert(binary_dilation(corner,3,3,out));
    for(size_t i=0;i<9;++i) assert(out[i]==expected[i]);
    assert(binary_dilation(row,1,3,out));
    for(size_t i=0;i<3;++i) assert(out[i]==1);
    assert(binary_dilation(zero,1,1,out) && out[0]==0);
    assert(binary_dilation(one,1,1,out) && out[0]==1);
    out[0]=99;
    assert(!binary_dilation(center,0,3,out));
    assert(!binary_dilation(center,3,0,out));
    assert(!binary_dilation(center,65,3,out));
    assert(!binary_dilation(center,3,65,out));
    assert(!binary_dilation(invalid,3,3,out));
    assert(!binary_dilation(NULL,3,3,out));
    assert(!binary_dilation(center,3,3,NULL));
    assert(out[0]==99);
    puts("C binary dilation tests passed");
    return 0;
}
