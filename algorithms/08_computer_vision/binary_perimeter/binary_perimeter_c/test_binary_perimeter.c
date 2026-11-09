#include "binary_perimeter.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const uint8_t *image, size_t h,size_t w,uint32_t want) {
    uint32_t result=99999;
    assert(binary_perimeter(image,h,w,&result));
    assert(result==want);
}
int main(void) {
    const uint8_t one[]={1},zero[]={0},ones[]={1,1,1,1};
    const uint8_t zeros[]={0,0,0,0},gap[]={1,0,1},line[]={1,1,1};
    const uint8_t ring[]={1,1,1,1,0,1,1,1,1};
    const uint8_t center[]={0,0,0,0,1,0,0,0,0};
    const uint8_t diag[]={1,0,0,1};
    check(one,1,1,4);
    check(zero,1,1,0);
    check(ones,2,2,8);
    check(zeros,2,2,0);
    check(gap,1,3,8);
    check(line,1,3,8);
    check(line,3,1,8);
    check(ring,3,3,16);
    check(center,3,3,4);
    check(diag,2,2,8);
    uint8_t big[4096];
    memset(big,1,sizeof(big));
    check(big,64,64,256);
    for(size_t row=0;row<64;++row)
        for(size_t col=0;col<64;++col)
            big[row*64+col]=(uint8_t)((row+col)%2);
    check(big,64,64,8192);
    memset(big,0,sizeof(big));
    check(big,64,64,0);
    uint32_t result=777;
    const uint8_t invalid[]={0,2};
    assert(!binary_perimeter(invalid,1,2,&result));
    assert(!binary_perimeter(big,0,1,&result));
    assert(!binary_perimeter(big,1,0,&result));
    assert(!binary_perimeter(big,65,1,&result));
    assert(!binary_perimeter(big,1,65,&result));
    assert(!binary_perimeter(NULL,1,1,&result));
    assert(!binary_perimeter(big,1,1,NULL));
    assert(result==777);
    puts("C binary perimeter tests passed");
    return 0;
}
