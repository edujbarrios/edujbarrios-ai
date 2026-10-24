#include "decision_stump.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[] = {0,1,2,3};
    const uint8_t right_labels[] = {0,0,1,1};
    const uint8_t left_labels[] = {1,1,0,0};
    const double shuffled[] = {3,0,2,1};
    const uint8_t shuffle_labels[] = {1,0,1,0};
    const double constant[] = {4,4};
    const uint8_t positive[] = {1,1};
    const uint8_t zero[] = {0,0};
    const uint8_t invalid[] = {0,2};
    const double notfinite[] = {0,NAN};
    double threshold = -9;
    uint8_t orientation = 9;
    size_t errors = 99;
    uint8_t label = 9;
    assert(decision_stump(x,right_labels,4,&threshold,&orientation,&errors));
    assert(threshold==1 && orientation==1 && errors==0);
    assert(decision_stump_predict(2,threshold,orientation,&label) && label==1);
    assert(decision_stump(x,left_labels,4,&threshold,&orientation,&errors));
    assert(threshold==1 && orientation==0 && errors==0);
    assert(decision_stump(shuffled,shuffle_labels,4,&threshold,&orientation,&errors));
    assert(threshold==1 && orientation==1 && errors==0);
    assert(decision_stump(constant,positive,2,&threshold,&orientation,&errors));
    assert(threshold==4 && orientation==0 && errors==0);
    assert(decision_stump(constant,zero,2,&threshold,&orientation,&errors));
    assert(threshold==4 && orientation==1 && errors==0);
    threshold=-9; orientation=9; errors=99;
    assert(!decision_stump(x,invalid,2,&threshold,&orientation,&errors));
    assert(!decision_stump(notfinite,zero,2,&threshold,&orientation,&errors));
    assert(!decision_stump(x,right_labels,0,&threshold,&orientation,&errors));
    assert(!decision_stump_predict(INFINITY,1,1,&label));
    assert(!decision_stump_predict(1,1,2,&label));
    assert(threshold==-9 && orientation==9 && errors==99);
    puts("C decision stump tests passed");
    return 0;
}
