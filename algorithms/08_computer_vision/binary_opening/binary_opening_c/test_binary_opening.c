#include "binary_opening.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    uint8_t input[49]={0}, expected[49]={0}, out[49]={0};
    for (size_t y=2;y<=4;++y) for (size_t x=2;x<=4;++x) {
        input[y*7+x]=1;
        expected[y*7+x]=1;
    }
    input[0]=1; /* isolated noise */
    assert(binary_opening(input,49,7,7,out));
    for (size_t i=0;i<49;++i) assert(out[i]==expected[i]);
    assert(binary_opening(input,49,7,7,input)); /* in-place */
    for (size_t i=0;i<49;++i) assert(input[i]==expected[i]);
    uint8_t one[]={1}, empty[]={0}, bad[]={2}, output[]={9};
    assert(binary_opening(one,1,1,1,output) && output[0]==0);
    assert(binary_opening(empty,1,1,1,output) && output[0]==0);
    output[0]=9;
    assert(!binary_opening(bad,1,1,1,output) && output[0]==9);
    assert(!binary_opening(one,1,0,1,output) && output[0]==9);
    assert(!binary_opening(one,1,1,0,output) && output[0]==9);
    assert(!binary_opening(one,1,2,1,output) && output[0]==9);
    assert(!binary_opening(one,1,65,1,output) && output[0]==9);
    assert(!binary_opening(NULL,1,1,1,output) && output[0]==9);
    assert(!binary_opening(one,1,1,1,NULL));
    puts("C binary opening tests passed");
    return 0;
}
