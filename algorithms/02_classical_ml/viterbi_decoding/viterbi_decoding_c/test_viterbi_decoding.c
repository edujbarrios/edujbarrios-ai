#include "viterbi_decoding.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double initial[]={.6,.4};
    const double transition[]={.7,.3,.4,.6};
    const double emission[]={.5,.5,.1,.9};
    const uint8_t observations[]={0,1,0};
    const uint8_t ones[]={1,1,1};
    const uint8_t invalid_obs[]={0,2};
    const double uniform[]={.5,.5};
    const double uniform_m[]={.5,.5,.5,.5};
    const double invalid_prior[]={0,1};
    const double invalid_trans[]={.7,.4,.4,.6};
    uint8_t path[]={9,9,9};
    double score=-9;
    assert(viterbi_decoding(observations,3,initial,transition,emission,path,&score));
    assert(path[0]==0 && path[1]==0 && path[2]==0);
    assert(fabs(score-log(.03675))<1e-12);
    assert(viterbi_decoding(ones,3,initial,transition,emission,path,&score));
    assert(path[0]==1 && path[1]==1 && path[2]==1);
    assert(viterbi_decoding(observations,3,uniform,uniform_m,uniform_m,path,&score));
    assert(path[0]==0 && path[1]==0 && path[2]==0);
    assert(viterbi_decoding(observations,1,initial,transition,emission,path,&score));
    assert(path[0]==0);
    path[0]=9;score=-9;
    assert(!viterbi_decoding(observations,0,initial,transition,emission,path,&score));
    assert(!viterbi_decoding(observations,4097,initial,transition,emission,path,&score));
    assert(!viterbi_decoding(invalid_obs,2,initial,transition,emission,path,&score));
    assert(!viterbi_decoding(observations,3,invalid_prior,transition,emission,path,&score));
    assert(!viterbi_decoding(observations,3,initial,invalid_trans,emission,path,&score));
    assert(!viterbi_decoding(observations,3,initial,transition,emission,NULL,&score));
    assert(path[0]==9 && score==-9);
    puts("C Viterbi tests passed");
    return 0;
}
