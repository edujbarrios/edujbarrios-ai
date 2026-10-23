#include "scaled_dot_product_attention.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double q[]={1,0}, keys[]={1,0,0,1};
    const double vals[]={10,0,0,20};
    const double tie_q[]={1,1}, tie_vals[]={2,4,6,8};
    const double huge[]={1e308}, little[]={1};
    const double low_high[]={-1000,1000}, binary_vals[]={0,1};
    const double scalar[]={7}, bad[]={NAN};
    double out[]={-9,-9};
    const double e=exp(1.0/sqrt(2.0));
    const double weight=e/(e+1.0);
    assert(scaled_dot_product_attention(q,2,keys,vals,2,2,out));
    assert(fabs(out[0]-10*weight)<1e-12);
    assert(fabs(out[1]-20*(1-weight))<1e-12);
    assert(scaled_dot_product_attention(tie_q,2,keys,tie_vals,2,2,out));
    assert(out[0]==4 && out[1]==6);
    assert(scaled_dot_product_attention(little,1,little,scalar,1,1,out));
    assert(out[0]==7);
    assert(scaled_dot_product_attention(huge,1,low_high,binary_vals,2,1,out)==false);
    /* Very large, finite scores should not overflow exponentiation. */
    const double small_q[]={1000};
    assert(scaled_dot_product_attention(small_q,1,low_high,binary_vals,2,1,out));
    assert(out[0]==1);
    out[0]=-9;
    assert(!scaled_dot_product_attention(q,0,keys,vals,2,2,out));
    assert(!scaled_dot_product_attention(q,2,keys,vals,0,2,out));
    assert(!scaled_dot_product_attention(bad,1,little,scalar,1,1,out));
    assert(!scaled_dot_product_attention(huge,1,huge,scalar,1,1,out));
    assert(!scaled_dot_product_attention(q,2,keys,vals,2,2,NULL));
    assert(out[0]==-9);
    puts("C scaled dot product attention tests passed");
    return 0;
}
