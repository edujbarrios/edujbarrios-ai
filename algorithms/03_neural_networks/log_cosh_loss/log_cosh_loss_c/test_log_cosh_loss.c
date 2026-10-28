#include "log_cosh_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double t[]={0,1,2},p[]={0,2,0},same[]={0,1,2};
    const double high[]={1000000.0},low[]={-1000000.0};
    const double bad[]={0,NAN,2},inf[]={0,INFINITY,2},large[]={0,1000001,2};
    double out=99.0;
    assert(log_cosh_loss(t,p,3,&out) && fabs(out-0.5862611926136305)<1e-12);
    assert(log_cosh_loss(t,same,3,&out) && out<1e-12);
    assert(log_cosh_loss(high,low,1,&out) && fabs(out-(2000000.0-log(2.0)))<1e-8);
    out=99.0;
    assert(!log_cosh_loss(t,p,0,&out));
    assert(!log_cosh_loss(t,p,10001,&out));
    assert(!log_cosh_loss(t,bad,3,&out));
    assert(!log_cosh_loss(t,inf,3,&out));
    assert(!log_cosh_loss(t,large,3,&out));
    assert(!log_cosh_loss(NULL,p,3,&out));
    assert(!log_cosh_loss(t,p,3,NULL));
    assert(out==99.0);
    puts("C log-cosh tests passed");
    return 0;
}
