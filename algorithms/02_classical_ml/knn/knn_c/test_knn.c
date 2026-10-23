#include "knn.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double samples[] = {0,0, 5,5, 1,1, 0,2};
    const double query[] = {0,1};
    const double one[] = {1};
    const double nan_input[] = {NAN};
    const double huge[] = {1e308};
    const double neg_huge[] = {-1e308};
    size_t result[] = {99, 99, 99, 99};

    assert(knn(samples,4,2,query,2,result));
    assert(result[0] == 0 && result[1] == 3);
    assert(knn(samples,4,2,query,4,result));
    assert(result[0] == 0 && result[1] == 3 && result[2] == 2 && result[3] == 1);
    result[0] = 99;
    assert(!knn(samples,4,2,query,0,result));
    assert(!knn(samples,4,2,query,5,result));
    assert(!knn(samples,0,2,query,1,result));
    assert(!knn(nan_input,1,1,one,1,result));
    assert(!knn(huge,1,1,neg_huge,1,result));
    assert(result[0] == 99);
    puts("C KNN tests passed");
    return 0;
}
