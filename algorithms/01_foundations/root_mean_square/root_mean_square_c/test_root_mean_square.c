#include "root_mean_square.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *x, size_t n, double want) {
    double got = -999;
    assert(root_mean_square(x, n, &got));
    assert(fabs(got-want) <= 1e-11 * fmax(fabs(want), 1e-300));
}
int main(void) {
    const double pair[]={3,4}, negative[]={-3,4};
    const double zeros[]={0,0,0}, single[]={-42};
    const double huge[]={1e308,-1e308}, tiny[]={1e-300,-1e-300};
    const double triple[]={1,2,2};
    check(pair,2,sqrt(12.5));
    check(negative,2,sqrt(12.5));
    check(zeros,3,0);
    check(single,1,42);
    check(huge,2,1e308);
    check(tiny,2,1e-300);
    check(triple,3,sqrt(3.0));
    double maximum[512];
    for (size_t i=0;i<512;++i) maximum[i]=3;
    check(maximum,512,3);
    double got=777;
    const double nan[]={1,NAN}, inf[]={1,INFINITY};
    assert(!root_mean_square(pair,0,&got));
    assert(!root_mean_square(pair,513,&got));
    assert(!root_mean_square(NULL,1,&got));
    assert(!root_mean_square(pair,2,NULL));
    assert(!root_mean_square(nan,2,&got));
    assert(!root_mean_square(inf,2,&got));
    assert(got==777);
    puts("C RMS tests passed");
    return 0;
}
