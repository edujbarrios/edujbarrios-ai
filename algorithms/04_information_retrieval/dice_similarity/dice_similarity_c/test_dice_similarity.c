#include "dice_similarity.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const int32_t a[] = {1, 1, 2, 3}, b[] = {2, 3, 4};
    const int32_t same1[] = {INT32_MIN, 3}, same2[] = {3, INT32_MIN};
    const int32_t opposite[] = {INT32_MAX};
    double score = -1;
    assert(dice_similarity(a, 4, b, 3, &score));
    assert(fabs(score - 2.0 / 3.0) < 1e-12);
    assert(dice_similarity(NULL, 0, NULL, 0, &score) && score == 1);
    assert(dice_similarity(NULL, 0, a, 4, &score) && score == 0);
    assert(dice_similarity(a, 4, NULL, 0, &score) && score == 0);
    assert(dice_similarity(same1, 2, same2, 2, &score) && score == 1);
    assert(dice_similarity(same1, 2, opposite, 1, &score) && score == 0);
    score = -1;
    assert(!dice_similarity(NULL, 1, a, 4, &score));
    assert(!dice_similarity(a, 4, NULL, 1, &score));
    assert(!dice_similarity(a, 4, b, 3, NULL));
    assert(score == -1);
    puts("C Dice similarity tests passed");
    return 0;
}
