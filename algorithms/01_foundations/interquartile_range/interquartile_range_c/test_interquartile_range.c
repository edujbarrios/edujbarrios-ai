#include "interquartile_range.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const double *values,size_t n,double want) {
    double actual=-777;
    assert(interquartile_range(values,n,&actual));
    assert(fabs(actual-want)<=1e-11*fmax(fabs(want),1.0));
}
int main(void) {
    const double a[]={1,2,3,4},b[]={1,2,3,4,100},c[]={1,5};
    const double d[]={1,2,3},single[]={42},unsorted[]={100,4,3,2,1};
    const double signed_data[]={-5,-3,-1,1,3},ties[]={1,1,1,50,50,50};
    const double extremes[]={-1e308,1e308};
    check(a,4,1.5);
    check(b,5,2.);
    check(c,2,2.);
    check(d,3,1.);
    check(single,1,0.);
    check(unsorted,5,2.);
    check(signed_data,5,4.);
    check(ties,6,49.);
    check(extremes,2,1e308);
    double maximum[512];
    for(size_t i=0;i<512;++i)maximum[i]=7.;
    check(maximum,512,0.);
    double output=777;
    const double bad_nan[]={NAN},bad_inf[]={INFINITY};
    const double overflow[]={-1e308,-1e308,1e308,1e308};
    assert(!interquartile_range(NULL,1,&output));
    assert(!interquartile_range(a,0,&output));
    assert(!interquartile_range(a,513,&output));
    assert(!interquartile_range(a,4,NULL));
    assert(!interquartile_range(bad_nan,1,&output));
    assert(!interquartile_range(bad_inf,1,&output));
    assert(!interquartile_range(overflow,4,&output));
    assert(output==777);
    puts("C interquartile range tests passed");
    return 0;
}
