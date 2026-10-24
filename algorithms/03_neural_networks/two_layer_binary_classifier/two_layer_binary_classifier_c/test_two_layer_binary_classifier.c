#include "two_layer_binary_classifier.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[]={0,1,2,3}, y[]={0,0,1,1};
    const double single[]={0}, positive[]={1};
    const double dead[]={-1};
    const double bad[]={2};
    const double nonfinite[]={NAN};
    const double extreme[]={1e308};
    const double negative[]={0};
    double params[4]={-9,-9,-9,-9};
    double prob=-9;
    const double error = 1.0/(1.0+exp(-0.5)) - 1.0;
    assert(two_layer_binary_classifier(single,positive,1,0.2,1,params));
    assert(fabs(params[0]-1.0)<1e-12);
    assert(fabs(params[1]-(0.5-0.2*error))<1e-12);
    assert(fabs(params[2]-(1.0-0.2*error*0.5))<1e-12);
    assert(fabs(params[3]-(-0.2*error))<1e-12);
    assert(two_layer_binary_classifier(dead,positive,1,0.2,1,params));
    assert(params[0]==1.0 && params[1]==0.5 && params[2]==1.0);
    assert(two_layer_binary_classifier(x,y,4,0.1,500,params));
    assert(two_layer_predict(params,0,&prob) && prob<0.1);
    assert(two_layer_predict(params,3,&prob) && prob>0.9);
    params[0]=-9; prob=-9;
    assert(!two_layer_binary_classifier(x,bad,1,0.2,1,params));
    assert(!two_layer_binary_classifier(nonfinite,positive,1,0.2,1,params));
    assert(!two_layer_binary_classifier(x,y,4,0.0,1,params));
    assert(!two_layer_binary_classifier(x,y,4,0.2,0,params));
    assert(!two_layer_binary_classifier(extreme,negative,1,1e308,2,params));
    assert(!two_layer_predict(params,INFINITY,&prob));
    assert(params[0]==-9 && prob==-9);
    puts("C two-layer classifier tests passed");
    return 0;
}
