#include "gini_impurity.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const uint32_t pure[]={5,0},balanced[]={1,1},unequal[]={3,1};
    const uint32_t triple[]={1,1,1},maximal[]={1000000,1000000};
    const uint32_t one[]={8},zeros[]={0,0},big[]={0,1000001};
    double result=99.0;
    assert(gini_impurity(pure,2,&result) && result==0.0);
    assert(gini_impurity(balanced,2,&result) && result==0.5);
    assert(gini_impurity(unequal,2,&result) && result==0.375);
    assert(gini_impurity(triple,3,&result) && fabs(result-2.0/3.0)<1e-12);
    assert(gini_impurity(maximal,2,&result) && result==0.5);
    assert(gini_impurity(one,1,&result) && result==0.0);
    result=99.0;
    assert(!gini_impurity(zeros,2,&result));
    assert(!gini_impurity(big,2,&result));
    assert(!gini_impurity(one,0,&result));
    assert(!gini_impurity(one,129,&result));
    assert(!gini_impurity(NULL,2,&result));
    assert(!gini_impurity(one,1,NULL));
    assert(result==99.0);
    puts("C Gini impurity tests passed");
    return 0;
}
