#include "binary_hole_filling.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const uint8_t *input, const uint8_t *want, size_t h, size_t w) {
    uint8_t output[4096] = {0};
    assert(binary_hole_filling(input,h,w,output));
    for (size_t i=0;i<h*w;++i) assert(output[i]==want[i]);
}
int main(void) {
    const uint8_t center[] = {1,1,1,1,0,1,1,1,1};
    const uint8_t all_one[] = {1,1,1,1,1,1,1,1,1};
    check(center,all_one,3,3);
    const uint8_t diagonal[] = {0,1,1,1,0,1,1,1,1};
    const uint8_t diagonal_expected[] = {0,1,1,1,1,1,1,1,1};
    check(diagonal,diagonal_expected,3,3);
    const uint8_t zeros[] = {0,0,0,0}, ones[] = {1,1,1,1};
    check(zeros,zeros,2,2);
    check(ones,ones,2,2);
    const uint8_t singleton_zero[] = {0}, singleton_one[] = {1};
    check(singleton_zero,singleton_zero,1,1);
    check(singleton_one,singleton_one,1,1);
    const uint8_t line[] = {1,0,1,0};
    check(line,line,1,4);
    check(line,line,4,1);
    const uint8_t donut[] = {
        0,0,0,0,0, 0,1,1,1,0, 0,1,0,1,0,
        0,1,1,1,0, 0,0,0,0,0
    };
    uint8_t donut_expected[25];
    memcpy(donut_expected,donut,sizeof(donut));
    donut_expected[12]=1;
    check(donut,donut_expected,5,5);
    uint8_t big[4096];
    memset(big,1,sizeof(big)); big[32*64+32]=0;
    uint8_t output[4096]={0};
    assert(binary_hole_filling(big,64,64,output));
    for(size_t i=0;i<4096;++i)assert(output[i]==1);
    memset(big,0,sizeof(big));
    assert(binary_hole_filling(big,64,64,output));
    for(size_t i=0;i<4096;++i)assert(output[i]==0);
    output[0]=77;
    const uint8_t invalid[] = {0,2};
    assert(!binary_hole_filling(invalid,1,2,output));
    assert(!binary_hole_filling(big,0,1,output));
    assert(!binary_hole_filling(big,1,0,output));
    assert(!binary_hole_filling(big,65,1,output));
    assert(!binary_hole_filling(big,1,65,output));
    assert(!binary_hole_filling(NULL,1,1,output));
    assert(!binary_hole_filling(big,1,1,NULL));
    assert(output[0]==77);
    puts("C binary hole filling tests passed");
    return 0;
}
