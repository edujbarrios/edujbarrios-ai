#include "grayscale_contrast_stretching.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    uint8_t output[4096] = {0};
    const uint8_t pixels[] = {10,10,15,20};
    assert(grayscale_contrast_stretching(pixels,2,2,output));
    assert(output[0]==0 && output[1]==0 && output[2]==128 && output[3]==255);
    const uint8_t steps[] = {5,6,7,8};
    assert(grayscale_contrast_stretching(steps,1,4,output));
    assert(output[0]==0 && output[1]==85 && output[2]==170 && output[3]==255);
    const uint8_t constant[] = {99,99};
    assert(grayscale_contrast_stretching(constant,1,2,output));
    assert(output[0]==99 && output[1]==99);
    const uint8_t minmax[] = {0,255};
    assert(grayscale_contrast_stretching(minmax,1,2,output));
    assert(output[0]==0 && output[1]==255);
    const uint8_t midpoint[] = {0,1,2};
    assert(grayscale_contrast_stretching(midpoint,1,3,output));
    assert(output[0]==0 && output[1]==128 && output[2]==255);
    uint8_t image[4096];
    memset(image,50,sizeof(image));
    image[4095]=100;
    assert(grayscale_contrast_stretching(image,64,64,output));
    assert(output[0]==0 && output[4094]==0 && output[4095]==255);
    output[0]=77;
    assert(!grayscale_contrast_stretching(image,0,64,output));
    assert(!grayscale_contrast_stretching(image,64,0,output));
    assert(!grayscale_contrast_stretching(image,65,1,output));
    assert(!grayscale_contrast_stretching(image,1,65,output));
    assert(!grayscale_contrast_stretching(NULL,1,1,output));
    assert(!grayscale_contrast_stretching(image,1,1,NULL));
    assert(output[0]==77);
    puts("C contrast stretching tests passed");
    return 0;
}
