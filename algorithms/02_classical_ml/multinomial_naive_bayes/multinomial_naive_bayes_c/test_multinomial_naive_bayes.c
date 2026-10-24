#include "multinomial_naive_bayes.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint32_t counts[]={3,0,2,0,0,3,0,2};
    const uint8_t labels[]={0,0,1,1};
    const uint8_t one_class[]={0,0,0,0};
    const uint8_t bad_labels[]={0,0,1,2};
    const uint32_t bad_counts[]={1000001,0};
    const uint32_t q0[]={2,0},q1[]={0,2},qempty[]={0,0};
    double priors[]={-9,-9},likelihoods[4]={-9,-9,-9,-9};
    uint8_t predicted=9;
    assert(multinomial_naive_bayes(counts,labels,4,2,priors,likelihoods));
    assert(fabs(priors[0]-log(0.5))<1e-12 && fabs(priors[1]-log(0.5))<1e-12);
    assert(fabs(likelihoods[0]-log(6.0/7.0))<1e-12);
    assert(fabs(likelihoods[1]-log(1.0/7.0))<1e-12);
    assert(fabs(likelihoods[2]-log(1.0/7.0))<1e-12);
    assert(fabs(likelihoods[3]-log(6.0/7.0))<1e-12);
    assert(multinomial_naive_bayes_predict(priors,likelihoods,q0,2,&predicted)&&predicted==0);
    assert(multinomial_naive_bayes_predict(priors,likelihoods,q1,2,&predicted)&&predicted==1);
    assert(multinomial_naive_bayes_predict(priors,likelihoods,qempty,2,&predicted)&&predicted==0);
    priors[0]=-9;likelihoods[0]=-9;
    assert(!multinomial_naive_bayes(counts,one_class,4,2,priors,likelihoods));
    assert(!multinomial_naive_bayes(counts,bad_labels,4,2,priors,likelihoods));
    assert(!multinomial_naive_bayes(bad_counts,labels,1,2,priors,likelihoods));
    assert(!multinomial_naive_bayes(counts,labels,0,2,priors,likelihoods));
    assert(!multinomial_naive_bayes(counts,labels,4,0,priors,likelihoods));
    assert(priors[0]==-9 && likelihoods[0]==-9);
    predicted=9;
    assert(!multinomial_naive_bayes_predict(priors,likelihoods,bad_counts,2,&predicted));
    const double invalid[]={NAN,0};
    assert(!multinomial_naive_bayes_predict(invalid,likelihoods,q0,2,&predicted));
    assert(predicted==9);
    puts("C Multinomial Naive Bayes tests passed");
    return 0;
}
