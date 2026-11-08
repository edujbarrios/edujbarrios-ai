#include "grayscale_histogram_equalization.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    uint8_t result[4096] = {0};
    const uint8_t img[] = {10,10,20,30};
    assert(grayscale_histogram_equalization(img,2,2,result));
    assert(result[0]==0 && result[1]==0 && result[2]==128 && result[3]==255);
    const uint8_t constant[] = {20,20,20};
    assert(grayscale_histogram_equalization(constant,1,3,result));
    assert(result[0]==20 && result[1]==20 && result[2]==20);
    const uint8_t extremes[] = {0,255};
    assert(grayscale_histogram_equalization(extremes,1,2,result));
    assert(result[0]==0 && result[1]==255);
    const uint8_t five[] = {0,1,2,3,4};
    assert(grayscale_histogram_equalization(five,1,5,result));
    assert(result[0]==0 && result[1]==64 && result[2]==128 &&
           result[3]==191 && result[4]==255);
    uint8_t big[4096] = {0};
    big[4095] = 255;
    assert(grayscale_histogram_equalization(big,64,64,result));
    assert(result[0]==0 && result[4094]==0 && result[4095]==255);
    result[0]=88;
    assert(!grayscale_histogram_equalization(big,0,64,result));
    assert(!grayscale_histogram_equalization(big,65,1,result));
    assert(!grayscale_histogram_equalization(big,1,0,result));
    assert(!grayscale_histogram_equalization(big,1,65,result));
    assert(!grayscale_histogram_equalization(NULL,1,1,result));
    assert(!grayscale_histogram_equalization(big,1,1,NULL));
    assert(result[0]==88);
    puts("C histogram equalization tests passed");
    return 0;
}
