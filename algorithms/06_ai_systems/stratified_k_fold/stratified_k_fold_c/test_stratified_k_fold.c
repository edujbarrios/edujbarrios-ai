#include "stratified_k_fold.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint8_t binary[]={0,1,0,1,0,1};
    const uint8_t uneven[]={0,0,0,0,1,1,1,1};
    const uint8_t too_few[]={0,0,1,1,1,1};
    const uint8_t bad[]={0,1,0,2,0,1};
    uint32_t fold[]={99,99,99,99,99,99,99,99};
    assert(stratified_k_fold(binary,6,3,fold));
    assert(fold[0]==0 && fold[1]==0 && fold[2]==1 &&
           fold[3]==1 && fold[4]==2 && fold[5]==2);
    assert(stratified_k_fold(uneven,8,3,fold));
    const uint32_t expected[]={0,1,2,0,1,2,0,1};
    for (size_t i=0;i<8;++i) assert(fold[i]==expected[i]);
    fold[0]=99;
    assert(!stratified_k_fold(binary,6,1,fold));
    assert(!stratified_k_fold(binary,6,4,fold));
    assert(!stratified_k_fold(too_few,6,3,fold));
    assert(!stratified_k_fold(bad,6,3,fold));
    assert(!stratified_k_fold(binary,0,3,fold));
    assert(!stratified_k_fold(binary,6,3,NULL));
    assert(fold[0]==99);
    puts("C stratified K-fold tests passed");
    return 0;
}
