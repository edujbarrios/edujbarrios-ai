#include "binary_dice_coefficient.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint8_t a[] = {1,1,0,0}, b[] = {1,0,1,0};
    const uint8_t z[] = {0,0,0,0}, bad[] = {0,2,0,0};
    const uint8_t one[] = {1,1,1,1};
    double score = -1;
    assert(binary_dice_coefficient(a,4,b,4,&score) && fabs(score-0.5)<1e-12);
    assert(binary_dice_coefficient(a,4,a,4,&score) && score==1);
    assert(binary_dice_coefficient(z,4,z,4,&score) && score==1);
    assert(binary_dice_coefficient(z,4,one,4,&score) && score==0);
    score = -1;
    assert(!binary_dice_coefficient(a,4,b,3,&score) && score==-1);
    assert(!binary_dice_coefficient(a,0,b,0,&score) && score==-1);
    assert(!binary_dice_coefficient(a,4,bad,4,&score) && score==-1);
    assert(!binary_dice_coefficient(NULL,4,b,4,&score) && score==-1);
    assert(!binary_dice_coefficient(a,4,b,4,NULL));
    puts("C binary Dice tests passed");
    return 0;
}
