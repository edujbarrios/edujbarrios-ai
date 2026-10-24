#include "rotary_positional_embedding.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double query[]={1,0}, key[]={0,1};
    const double ones[]={1,0,1,0};
    const double zero_pos_q[]={3,4}, zero_pos_k[]={5,6};
    const double huge[]={1.7e308,1.7e308};
    const double bad[]={NAN,0};
    double qr[]={-9,-9,-9,-9},kr[]={-9,-9,-9,-9};
    assert(rotary_positional_embedding(query,key,2,1,0,qr,kr));
    assert(fabs(qr[0]-cos(1))<1e-12);
    assert(fabs(qr[1]-sin(1))<1e-12);
    assert(fabs(kr[0])<1e-12 && fabs(kr[1]-1)<1e-12);
    assert(rotary_positional_embedding(zero_pos_q,zero_pos_k,2,0,0,qr,kr));
    assert(qr[0]==3 && qr[1]==4 && kr[0]==5 && kr[1]==6);
    assert(rotary_positional_embedding(ones,ones,4,1,1,qr,kr));
    assert(fabs(qr[0]-cos(1))<1e-12 && fabs(qr[1]-sin(1))<1e-12);
    assert(fabs(qr[2]-cos(0.01))<1e-12 && fabs(qr[3]-sin(0.01))<1e-12);
    assert(fabs(qr[0]-kr[0])<1e-12 && fabs(qr[1]-kr[1])<1e-12);
    qr[0]=-9;kr[0]=-9;
    assert(!rotary_positional_embedding(query,key,1,1,0,qr,kr));
    assert(!rotary_positional_embedding(query,key,2,1000001,0,qr,kr));
    assert(!rotary_positional_embedding(bad,key,2,1,0,qr,kr));
    assert(!rotary_positional_embedding(huge,key,2,1,0,qr,kr));
    assert(!rotary_positional_embedding(query,key,2,0,0,NULL,kr));
    assert(qr[0]==-9 && kr[0]==-9);
    puts("C rotary positional embedding tests passed");
    return 0;
}
