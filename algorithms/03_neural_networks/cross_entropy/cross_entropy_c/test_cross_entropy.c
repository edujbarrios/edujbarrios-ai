#include "cross_entropy.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double equal[] = {0,0};
    const double strong[] = {1000,0};
    const double bad[] = {NAN,0};
    double out = -100.0;
    assert(cross_entropy(equal,2,0,&out) && fabs(out-log(2.0))<1e-12);
    assert(cross_entropy(strong,2,0,&out) && fabs(out)<1e-12);
    assert(cross_entropy(strong,2,1,&out) && fabs(out-1000)<1e-12);
    out=-100.0;
    assert(!cross_entropy(equal,0,0,&out));
    assert(!cross_entropy(equal,2,2,&out));
    assert(!cross_entropy(bad,2,0,&out));
    assert(!cross_entropy(equal,2,0,NULL));
    assert(out==-100.0);
    puts("C cross entropy tests passed");
    return 0;
}
