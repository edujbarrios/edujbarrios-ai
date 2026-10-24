#include "gelu.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double input[]={0,1,-1,100,-100};
    const double invalid[]={NAN};
    const double huge[]={1e308};
    double values[]={-9,-9,-9,-9,-9},derivatives[]={-9,-9,-9,-9,-9};
    const double u=0.7978845608028654*(1.0+0.044715);
    const double t=tanh(u);
    const double expected=0.5*(1.0+t);
    const double derivative=expected+0.5*(1.0-t*t)*0.7978845608028654*(1.0+3.0*0.044715);
    assert(gelu(input,5,values,derivatives));
    assert(values[0]==0 && derivatives[0]==0.5);
    assert(fabs(values[1]-expected)<1e-12 && fabs(derivatives[1]-derivative)<1e-12);
    assert(fabs(values[2]-(expected-1.0))<1e-12);
    assert(fabs(derivatives[2]-(1.0-derivative))<1e-12);
    assert(values[3]==100 && derivatives[3]==1);
    assert(values[4]==0 && derivatives[4]==0);
    values[0]=-9;derivatives[0]=-9;
    assert(!gelu(input,0,values,derivatives));
    assert(!gelu(invalid,1,values,derivatives));
    assert(!gelu(huge,1,values,derivatives));
    assert(!gelu(input,5,NULL,derivatives));
    assert(values[0]==-9 && derivatives[0]==-9);
    puts("C GELU tests passed");
    return 0;
}
