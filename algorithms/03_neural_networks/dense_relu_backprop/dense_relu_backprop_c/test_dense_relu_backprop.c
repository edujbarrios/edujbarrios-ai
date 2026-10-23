#include "dense_relu_backprop.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={2,-1},w[]={1,1,-1,2},b[]={0,1},up[]={3,4};
    const double zero[]={0},scalar_w[]={3},scalar_b[]={0},scalar_up[]={5};
    const double bad[]={NAN},huge[]={1e308};
    double dx[]={-9,-9},dw[]={-9,-9,-9,-9},db[]={-9,-9};
    assert(dense_relu_backprop(x,2,w,b,up,2,dx,dw,db));
    assert(dx[0]==3 && dx[1]==3);
    assert(dw[0]==6 && dw[1]==-3 && dw[2]==0 && dw[3]==0);
    assert(db[0]==3 && db[1]==0);
    assert(dense_relu_backprop(zero,1,scalar_w,scalar_b,scalar_up,1,dx,dw,db));
    assert(dx[0]==0 && dw[0]==0 && db[0]==0);
    dx[0]=-9;dw[0]=-9;db[0]=-9;
    assert(!dense_relu_backprop(x,0,w,b,up,2,dx,dw,db));
    assert(!dense_relu_backprop(x,2,w,b,up,0,dx,dw,db));
    assert(!dense_relu_backprop(bad,1,scalar_w,scalar_b,scalar_up,1,dx,dw,db));
    assert(!dense_relu_backprop(huge,1,huge,scalar_b,scalar_up,1,dx,dw,db));
    assert(!dense_relu_backprop(x,2,w,b,up,2,NULL,dw,db));
    assert(dx[0]==-9 && dw[0]==-9 && db[0]==-9);
    puts("C dense ReLU backprop tests passed");
    return 0;
}
