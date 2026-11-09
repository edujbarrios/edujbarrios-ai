#include "harmonic_mean.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *x, size_t n, double expected) {
    double actual = -777;
    assert(harmonic_mean(x, n, &actual));
    assert(fabs(actual - expected) <= 1e-11 * fmax(fabs(expected), 1e-320));
}
int main(void) {
    const double pair[] = {2,6}, triple[] = {1,2,4};
    const double zeros[] = {0,2,6}, single[] = {42};
    const double imbalanced[] = {1e-300,1e300};
    const double tiny[] = {1e-320,1e-320};
    const double huge[] = {1e308,1e308};
    check(pair,2,3);
    check(triple,3,12.0/7.0);
    check(zeros,3,0);
    check(single,1,42);
    check(imbalanced,2,2e-300);
    check(tiny,2,1e-320);
    check(huge,2,1e308);
    double large[512];
    for (size_t i=0;i<512;++i) large[i]=7;
    check(large,512,7);
    double result=777;
    const double negative[]={-1,2}, missing[]={NAN,0}, infinite[]={INFINITY,1};
    assert(!harmonic_mean(pair,0,&result));
    assert(!harmonic_mean(pair,513,&result));
    assert(!harmonic_mean(NULL,1,&result));
    assert(!harmonic_mean(pair,2,NULL));
    assert(!harmonic_mean(negative,2,&result));
    assert(!harmonic_mean(missing,2,&result));
    assert(!harmonic_mean(infinite,2,&result));
    assert(result==777);
    puts("C harmonic mean tests passed");
    return 0;
}
