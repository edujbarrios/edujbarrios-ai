#include "binary_projection_profiles.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const uint8_t *p,size_t h,size_t w,const uint16_t *want) {
    uint16_t output[128]={0};
    assert(binary_projection_profiles(p,h,w,output));
    for(size_t i=0;i<h+w;++i)assert(output[i]==want[i]);
}
int main(void) {
    const uint8_t rect[]={1,0,1,0,1,1},line[]={1,0,1,1};
    const uint8_t zeros[]={0,0,0,0},ones[]={1,1,1,1};
    const uint8_t single[]={1},single_zero[]={0};
    const uint16_t want_rect[]={2,2,1,1,2};
    const uint16_t want_line[]={3,1,0,1,1};
    const uint16_t want_column[]={1,0,1,1,3};
    const uint16_t want_zeros[]={0,0,0,0},want_ones[]={2,2,2,2};
    const uint16_t want_single[]={1,1},want_zero[]={0,0};
    check(rect,2,3,want_rect);
    check(line,1,4,want_line);
    check(line,4,1,want_column);
    check(zeros,2,2,want_zeros);
    check(ones,2,2,want_ones);
    check(single,1,1,want_single);
    check(single_zero,1,1,want_zero);
    uint8_t big[4096];
    uint16_t out[128]={0};
    memset(big,1,sizeof(big));
    assert(binary_projection_profiles(big,64,64,out));
    for(size_t i=0;i<128;++i)assert(out[i]==64);
    memset(big,0,sizeof(big));
    assert(binary_projection_profiles(big,64,64,out));
    for(size_t i=0;i<128;++i)assert(out[i]==0);
    for(size_t r=0;r<64;++r)
        for(size_t c=0;c<64;++c)big[r*64+c]=(uint8_t)((r+c)%2);
    assert(binary_projection_profiles(big,64,64,out));
    for(size_t i=0;i<128;++i)assert(out[i]==32);
    out[0]=777;
    const uint8_t invalid[]={0,2};
    assert(!binary_projection_profiles(invalid,1,2,out));
    assert(!binary_projection_profiles(big,0,1,out));
    assert(!binary_projection_profiles(big,1,0,out));
    assert(!binary_projection_profiles(big,65,1,out));
    assert(!binary_projection_profiles(big,1,65,out));
    assert(!binary_projection_profiles(NULL,1,1,out));
    assert(!binary_projection_profiles(big,1,1,NULL));
    assert(out[0]==777);
    puts("C projection profile tests passed");
    return 0;
}
