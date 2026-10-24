#include "expected_calibration_error.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double p[]={.9,.8,.2,.6};
    const uint8_t y[]={1,0,0,1};
    const double perfect[]={1,0},half[]={.5,.5};
    const uint8_t perfect_labels[]={1,0},tie_labels[]={0,1};
    const double bad[]={1.1,.8},nan[]={NAN,.5};
    const uint8_t bad_labels[]={0,2};
    double score=-9;
    assert(expected_calibration_error(p,y,4,2,&score));
    assert(fabs(score-.025)<1e-12);
    assert(expected_calibration_error(p,y,4,4,&score));
    assert(fabs(score-.225)<1e-12);
    assert(expected_calibration_error(perfect,perfect_labels,2,10,&score));
    assert(score==0.0);
    assert(expected_calibration_error(half,tie_labels,2,1,&score));
    assert(score==0.0);
    score=-9;
    assert(!expected_calibration_error(p,y,0,2,&score));
    assert(!expected_calibration_error(p,y,4,0,&score));
    assert(!expected_calibration_error(p,y,4,101,&score));
    assert(!expected_calibration_error(bad,y,2,2,&score));
    assert(!expected_calibration_error(nan,y,2,2,&score));
    assert(!expected_calibration_error(perfect,bad_labels,2,2,&score));
    assert(!expected_calibration_error(p,y,4,2,NULL));
    assert(score==-9);
    puts("C expected calibration error tests passed");
    return 0;
}
