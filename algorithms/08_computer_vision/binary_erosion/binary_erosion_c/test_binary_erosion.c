#include "binary_erosion.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint8_t full[]={1,1,1,1,1,1,1,1,1};
    const uint8_t zero[]={0,0,0,0,0,0,0,0,0};
    const uint8_t broken[]={1,1,1,1,0,1,1,1,1};
    const uint8_t wrong[]={1,1,1,1,2,1,1,1,1};
    const uint8_t row[]={1,1,1};
    uint8_t out[9]={99,99,99,99,99,99,99,99,99};
    assert(binary_erosion(full,3,3,out));
    for(size_t i=0;i<9;++i) assert(out[i]==(i==4 ? 1:0));
    assert(binary_erosion(zero,3,3,out));
    for(size_t i=0;i<9;++i) assert(out[i]==0);
    assert(binary_erosion(broken,3,3,out));
    for(size_t i=0;i<9;++i) assert(out[i]==0);
    assert(binary_erosion(row,1,3,out));
    for(size_t i=0;i<3;++i) assert(out[i]==0);
    assert(binary_erosion(full,1,1,out) && out[0]==0);
    out[0]=99;
    assert(!binary_erosion(full,0,3,out));
    assert(!binary_erosion(full,3,0,out));
    assert(!binary_erosion(full,65,3,out));
    assert(!binary_erosion(full,3,65,out));
    assert(!binary_erosion(wrong,3,3,out));
    assert(!binary_erosion(NULL,3,3,out));
    assert(!binary_erosion(full,3,3,NULL));
    assert(out[0]==99);
    puts("C erosion tests passed");
    return 0;
}
