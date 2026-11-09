#include "binary_distance_transform.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    uint16_t result[4096] = {0};
    const uint8_t center[] = {1,1,1,1,0,1,1,1,1};
    const uint16_t want[] = {2,1,2,1,0,1,2,1,2};
    assert(binary_distance_transform(center,3,3,result));
    for (size_t i = 0; i < 9; ++i) assert(result[i] == want[i]);
    const uint8_t corner[] = {0,1,1,1};
    assert(binary_distance_transform(corner,2,2,result));
    assert(result[0]==0 && result[1]==1 && result[2]==1 && result[3]==2);
    const uint8_t all_one[] = {1,1,1,1};
    assert(binary_distance_transform(all_one,2,2,result));
    for (size_t i=0;i<4;++i) assert(result[i]==4);
    const uint8_t all_zero[] = {0,0,0,0};
    assert(binary_distance_transform(all_zero,2,2,result));
    for (size_t i=0;i<4;++i) assert(result[i]==0);
    const uint8_t one[] = {1}, zero[] = {0};
    assert(binary_distance_transform(one,1,1,result) && result[0]==2);
    assert(binary_distance_transform(zero,1,1,result) && result[0]==0);
    const uint8_t line[] = {1,0,1,1};
    assert(binary_distance_transform(line,1,4,result));
    assert(result[0]==1 && result[1]==0 && result[2]==1 && result[3]==2);
    assert(binary_distance_transform(line,4,1,result));
    assert(result[0]==1 && result[1]==0 && result[2]==1 && result[3]==2);
    uint8_t maximum[4096];
    memset(maximum,1,sizeof(maximum));maximum[0]=0;
    assert(binary_distance_transform(maximum,64,64,result));
    assert(result[0]==0 && result[1]==1 && result[64]==1 && result[4095]==126);
    memset(maximum,1,sizeof(maximum));
    assert(binary_distance_transform(maximum,64,64,result));
    assert(result[0]==128 && result[4095]==128);
    result[0]=777;
    const uint8_t invalid[]={0,2};
    assert(!binary_distance_transform(invalid,1,2,result));
    assert(!binary_distance_transform(one,0,1,result));
    assert(!binary_distance_transform(one,1,0,result));
    assert(!binary_distance_transform(one,65,1,result));
    assert(!binary_distance_transform(one,1,65,result));
    assert(!binary_distance_transform(NULL,1,1,result));
    assert(!binary_distance_transform(one,1,1,NULL));
    assert(result[0]==777);
    puts("C binary distance transform tests passed");
    return 0;
}
