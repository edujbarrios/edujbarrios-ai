#include "otsu_threshold.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint8_t pairs[]={10,20,200,210};
    const uint8_t extremes[]={0,0,255,255};
    const uint8_t adjacent[]={1,2};
    const uint8_t equal[]={7,7,7};
    const uint8_t single[]={255};
    uint8_t result=99;
    assert(otsu_threshold(pairs,4,&result) && result==20);
    assert(otsu_threshold(extremes,4,&result) && result==0);
    assert(otsu_threshold(adjacent,2,&result) && result==1);
    assert(otsu_threshold(equal,3,&result) && result==0);
    assert(otsu_threshold(single,1,&result) && result==0);
    result=99;
    assert(!otsu_threshold(pairs,0,&result));
    assert(!otsu_threshold(pairs,4097,&result));
    assert(!otsu_threshold(NULL,4,&result));
    assert(!otsu_threshold(pairs,4,NULL));
    assert(result==99);
    puts("C Otsu threshold tests passed");
    return 0;
}
