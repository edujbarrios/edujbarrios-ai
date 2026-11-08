#include "weighted_median.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *v, const double *w, size_t n, double expected) {
    double out = 777;
    assert(weighted_median(v, w, n, &out));
    assert(out == expected);
}

int main(void) {
    const double v1[] = {1,2,10}, w1[] = {1,1,8};
    const double v2[] = {1,2,3,4}, w2[] = {1,1,1,1};
    const double v3[] = {10,1,3}, w3[] = {0,2,1};
    const double v4[] = {1,100}, w4[] = {0,2};
    const double v5[] = {-5,2}, w5[] = {1,1};
    const double v6[] = {-1e308,1e308}, w6[] = {1,1};
    const double tiny[] = {0,5e-324};
    check(v1,w1,3,10);
    check(v2,w2,4,2);
    check(v3,w3,3,1);
    check(v4,w4,2,100);
    check(v5,w5,2,-5);
    check(v6,w6,2,-1e308);
    check(v4,tiny,2,100);
    double long_v[512], long_w[512];
    for (size_t i = 0; i < 512; ++i) { long_v[i] = 3; long_w[i] = 1; }
    check(long_v,long_w,512,3);
    double out = 777;
    const double zero[] = {0,0}, negative[] = {-1,1};
    const double overflow[] = {1e308,1e308};
    const double invalid[] = {NAN,1}, infinite[] = {1,INFINITY};
    assert(!weighted_median(v2,w2,0,&out));
    assert(!weighted_median(v2,w2,513,&out));
    assert(!weighted_median(NULL,w1,3,&out));
    assert(!weighted_median(v1,NULL,3,&out));
    assert(!weighted_median(v1,w1,3,NULL));
    assert(!weighted_median(v4,zero,2,&out));
    assert(!weighted_median(v4,negative,2,&out));
    assert(!weighted_median(v4,overflow,2,&out));
    assert(!weighted_median(invalid,w4,2,&out));
    assert(!weighted_median(v4,infinite,2,&out));
    assert(out == 777);
    puts("C weighted median tests passed");
    return 0;
}
