#include "adam_optimizer.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double params[] = {2.0, -3.0}, grad[] = {1.0, -2.0};
    const double zeros[] = {0.0, 0.0};
    const double negative[] = {-1.0, 0.0};
    const double bad[] = {NAN, 0.0};
    const double overflow[] = {1e308, 0.0};
    double p[2] = {-9,-9}, m[2] = {-9,-9}, v[2] = {-9,-9};
    assert(adam_optimizer(params,grad,zeros,zeros,2,0.1,1,p,m,v));
    assert(fabs(p[0] - (2.0 - 0.1/(1+1e-8))) < 1e-12);
    assert(fabs(p[1] - (-3.0 + 0.2/(2+1e-8))) < 1e-12);
    assert(fabs(m[0]-0.1)<1e-12 && fabs(m[1]+0.2)<1e-12);
    assert(fabs(v[0]-0.001)<1e-12 && fabs(v[1]-0.004)<1e-12);
    const double first_p[] = {p[0],p[1]}, first_m[] = {m[0],m[1]}, first_v[] = {v[0],v[1]};
    assert(adam_optimizer(first_p,zeros,first_m,first_v,2,0.1,2,p,m,v));
    assert(m[0] < first_m[0] && m[1] > first_m[1]);
    p[0]=-9;m[0]=-9;v[0]=-9;
    assert(!adam_optimizer(params,grad,zeros,negative,2,0.1,1,p,m,v));
    assert(!adam_optimizer(params,bad,zeros,zeros,2,0.1,1,p,m,v));
    assert(!adam_optimizer(params,overflow,zeros,zeros,2,0.1,1,p,m,v));
    assert(!adam_optimizer(params,grad,zeros,zeros,2,0.1,0,p,m,v));
    assert(!adam_optimizer(params,grad,zeros,zeros,0,0.1,1,p,m,v));
    assert(!adam_optimizer(params,grad,zeros,zeros,2,0.0,1,p,m,v));
    assert(p[0]==-9 && m[0]==-9 && v[0]==-9);
    puts("C Adam optimizer tests passed");
    return 0;
}
