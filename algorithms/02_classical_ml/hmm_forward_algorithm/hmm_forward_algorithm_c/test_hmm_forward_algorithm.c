#include "hmm_forward_algorithm.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double initial[]={.6,.4};
    const double transition[]={.7,.3,.4,.6};
    const double emission[]={.5,.5,.1,.9};
    const uint8_t obs[]={0,1,0};
    const double uniform[]={.5,.5};
    const double uniform_mat[]={.5,.5,.5,.5};
    const double invalid_init[]={0,1};
    const double invalid_trans[]={.7,.4,.4,.6};
    const double invalid_emit[]={.5,.5,0,1};
    const uint8_t wrong[]={0,2};
    uint8_t long_seq[100];
    for (size_t i=0;i<100;++i) long_seq[i]=(uint8_t)(i%2);
    double value=-9;
    assert(hmm_forward_algorithm(obs,3,initial,transition,emission,&value));
    assert(fabs(value-log(.069616))<1e-12);
    assert(hmm_forward_algorithm(obs,1,initial,transition,emission,&value));
    assert(fabs(value-log(.34))<1e-12);
    assert(hmm_forward_algorithm(long_seq,100,uniform,uniform_mat,uniform_mat,&value));
    assert(fabs(value-100.0*log(.5))<1e-11);
    value=-9;
    assert(!hmm_forward_algorithm(obs,0,initial,transition,emission,&value));
    assert(!hmm_forward_algorithm(obs,4097,initial,transition,emission,&value));
    assert(!hmm_forward_algorithm(wrong,2,initial,transition,emission,&value));
    assert(!hmm_forward_algorithm(obs,3,invalid_init,transition,emission,&value));
    assert(!hmm_forward_algorithm(obs,3,initial,invalid_trans,emission,&value));
    assert(!hmm_forward_algorithm(obs,3,initial,transition,invalid_emit,&value));
    assert(!hmm_forward_algorithm(obs,3,initial,transition,emission,NULL));
    assert(value==-9);
    puts("C HMM Forward tests passed");
    return 0;
}
