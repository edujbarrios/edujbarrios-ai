#include "causal_attention.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double queries[]={0,0,0},keys[]={1,2,3};
    const double values[]={2,4,6,8,10,12};
    const double one[]={3},one_value[]={7};
    const double large_queries[]={1000,1000};
    const double large_keys[]={-1000,1000};
    const double large_values[]={0,7};
    const double huge[]={1e308};
    const double bad[]={NAN};
    double output[]={-9,-9,-9,-9,-9,-9};
    assert(causal_attention(queries,keys,values,3,1,2,output));
    assert(output[0]==2 && output[1]==4);
    assert(output[2]==4 && output[3]==6);
    assert(fabs(output[4]-6)<1e-12 && fabs(output[5]-8)<1e-12);
    assert(causal_attention(one,one,one_value,1,1,1,output));
    assert(output[0]==7);
    assert(causal_attention(large_queries,large_keys,large_values,2,1,1,output));
    assert(output[0]==0 && output[1]==7);
    output[0]=-9;
    assert(!causal_attention(huge,huge,one_value,1,1,1,output));
    assert(!causal_attention(bad,one,one_value,1,1,1,output));
    assert(!causal_attention(queries,keys,values,0,1,2,output));
    assert(!causal_attention(queries,keys,values,3,0,2,output));
    assert(!causal_attention(queries,keys,values,3,1,0,output));
    assert(!causal_attention(queries,keys,values,3,1,2,NULL));
    assert(output[0]==-9);
    puts("C causal attention tests passed");
    return 0;
}
