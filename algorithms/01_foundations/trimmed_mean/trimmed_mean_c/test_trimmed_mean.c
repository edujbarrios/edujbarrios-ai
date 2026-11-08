#include "trimmed_mean.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *values, size_t n, double fraction, double expected) {
    double result = -999;
    assert(trimmed_mean(values, n, fraction, &result));
    assert(fabs(result - expected) <= 1e-12 * fmax(1.0, fabs(expected)));
}
int main(void) {
    const double v1[] = {1,2,3,4,100};
    const double v2[] = {1,2,3,100};
    const double v3[] = {100,3,2,1};
    const double one[] = {42};
    const double odd[] = {1,2,3};
    const double far[] = {-1e308,1e308};
    const double v4[] = {0,2,100};
    check(v1,5,0.2,3);
    check(v2,4,0.25,2.5);
    check(v2,4,0,26.5);
    check(v3,4,0.25,2.5);
    check(one,1,0.49,42);
    check(odd,3,0.49,2);
    check(far,2,0,0);
    check(v4,3,0.3,34);
    double many[512];
    for (size_t i=0;i<512;++i) many[i]=7;
    check(many,512,0.49,7);
    double output=777;
    assert(!trimmed_mean(NULL,1,0,&output));
    assert(!trimmed_mean(v1,0,0,&output));
    assert(!trimmed_mean(v1,513,0,&output));
    assert(!trimmed_mean(v1,5,-0.1,&output));
    assert(!trimmed_mean(v1,5,0.5,&output));
    assert(!trimmed_mean(v1,5,INFINITY,&output));
    assert(!trimmed_mean(v1,5,NAN,&output));
    const double invalid[] = {1,NAN};
    assert(!trimmed_mean(invalid,2,0,&output));
    assert(!trimmed_mean(v1,5,0,NULL));
    assert(output==777);
    puts("C trimmed mean tests passed");
    return 0;
}
