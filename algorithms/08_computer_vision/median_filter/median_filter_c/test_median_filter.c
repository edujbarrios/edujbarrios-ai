#include "median_filter.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double impulse[9] = {0,0,0,0,16,0,0,0,0};
    const double expected[9] = {0,0,0,0,0,0,0,0,0};
    const double line[3] = {0,0,8}, line_expected[3] = {0,0,8};
    const double bad[4] = {0,0,NAN,0};
    const double excessive[4] = {0,0,1000001,0};
    const double constant[6] = {5,5,5,5,5,5};
    double result[9] = {99,99,99,99,99,99,99,99,99};
    assert(median_filter(impulse,3,3,result));
    for (size_t i=0; i<9; ++i) assert(fabs(result[i]-expected[i])<1e-12);
    assert(median_filter(line,1,3,result));
    for (size_t i=0; i<3; ++i) assert(fabs(result[i]-line_expected[i])<1e-12);
    assert(median_filter(constant,2,3,result));
    for (size_t i=0; i<6; ++i) assert(result[i]==5.0);
    assert(median_filter((const double[]){7},1,1,result));
    assert(result[0]==7.0);
    result[0]=99;
    assert(!median_filter(impulse,0,3,result));
    assert(!median_filter(impulse,3,65,result));
    assert(!median_filter(bad,2,2,result));
    assert(!median_filter(excessive,2,2,result));
    assert(!median_filter(NULL,2,2,result));
    assert(!median_filter(impulse,2,2,NULL));
    assert(result[0]==99);
    puts("C median_filter tests passed");
    return 0;
}
