#include "masked_attention.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double q[]={1,0};
    const double keys[]={1,0,0,1,1,1};
    const double values[]={10,0,0,20,4,6};
    const uint8_t mask[]={1,0,1};
    const uint8_t one[]={0,1,0};
    const uint8_t zeros[]={0,0,0};
    const uint8_t bad_mask[]={1,2,0};
    const double huge_query[]={1e308};
    const double huge_keys[]={1e308,1};
    const double huge_values[]={0,7};
    const uint8_t exclude_huge[]={0,1};
    const uint8_t include_huge[]={1,0};
    const double nonfinite_keys[]={NAN,1};
    double out[]={-9,-9};
    assert(masked_attention(q,2,keys,values,mask,3,2,out));
    assert(fabs(out[0]-7.0)<1e-12 && fabs(out[1]-3.0)<1e-12);
    assert(masked_attention(q,2,keys,values,one,3,2,out));
    assert(out[0]==0.0 && out[1]==20.0);
    assert(masked_attention(huge_query,1,huge_keys,huge_values,exclude_huge,2,1,out));
    assert(out[0]==7.0);
    out[0]=-9;
    assert(!masked_attention(q,2,keys,values,zeros,3,2,out));
    assert(!masked_attention(q,2,keys,values,bad_mask,3,2,out));
    assert(!masked_attention(huge_query,1,huge_keys,huge_values,include_huge,2,1,out));
    assert(!masked_attention(huge_query,1,nonfinite_keys,huge_values,exclude_huge,2,1,out));
    assert(!masked_attention(q,0,keys,values,mask,3,2,out));
    assert(!masked_attention(q,2,keys,values,mask,3,2,NULL));
    assert(out[0]==-9);
    puts("C masked_attention tests passed");
    return 0;
}
