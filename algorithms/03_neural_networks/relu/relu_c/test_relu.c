#include "relu.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {-2,-0.5,0,3};
    const double bad[] = {1,NAN};
    const double inf[] = {INFINITY};
    double out[] = {-9,-9,-9,-9};
    assert(relu(x,4,out));
    assert(out[0]==0 && out[1]==0 && out[2]==0 && out[3]==3);
    assert(relu(NULL,0,out));
    out[0]=-9; out[1]=-9;
    assert(!relu(bad,2,out) && out[0]==-9 && out[1]==-9);
    assert(!relu(inf,1,out) && out[0]==-9);
    assert(!relu(x,4,NULL));
    double in_place[] = {-1,2};
    assert(relu(in_place,2,in_place));
    assert(in_place[0]==0 && in_place[1]==2);
    puts("C ReLU tests passed");
    return 0;
}
