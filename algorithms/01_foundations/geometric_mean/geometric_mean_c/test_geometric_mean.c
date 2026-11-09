#include "geometric_mean.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void near(const double *v, size_t n, double expected) {
    double result = -777;
    assert(geometric_mean(v, n, &result));
    assert(fabs(result - expected) <= 1e-11 * fmax(1., fabs(expected)));
}
int main(void) {
    const double pair[] = {2,8}, ones[] = {1,1,1};
    const double zeros[] = {0,2,8}, limits[] = {1e-300,1e300};
    const double triples[] = {2,4,8}, half[] = {0.5,2};
    const double single[] = {42}, hugezero[] = {0,1e308};
    near(pair,2,4);
    near(ones,3,1);
    near(zeros,3,0);
    near(limits,2,1);
    near(triples,3,4);
    near(half,2,1);
    near(single,1,42);
    near(hugezero,2,0);
    double maximum[512];
    for(size_t i=0;i<512;++i)maximum[i]=7.;
    near(maximum,512,7);
    double out=777;
    const double invalid[] = {1,NAN};
    const double negative[] = {-0.01,2};
    const double infinite[] = {1,INFINITY};
    assert(!geometric_mean(NULL,1,&out));
    assert(!geometric_mean(pair,0,&out));
    assert(!geometric_mean(pair,513,&out));
    assert(!geometric_mean(pair,2,NULL));
    assert(!geometric_mean(invalid,2,&out));
    assert(!geometric_mean(negative,2,&out));
    assert(!geometric_mean(infinite,2,&out));
    assert(out==777);
    puts("C geometric mean tests passed");
    return 0;
}
