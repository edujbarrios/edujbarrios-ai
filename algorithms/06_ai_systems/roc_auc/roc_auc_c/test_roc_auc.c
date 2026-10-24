#include "roc_auc.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double scores[]={0.1,0.4,0.35,0.8};
    const uint8_t labels[]={0,0,1,1};
    const double ties[]={0.5,0.5},reversed[]={0.9,0.1},perfect[]={0.1,0.9};
    const double bad[]={0.5,NAN};
    const uint8_t binary[]={0,1},same[]={1,1},invalid[]={0,2};
    double result=-9;
    assert(roc_auc(scores,labels,4,&result) && fabs(result-.75)<1e-12);
    assert(roc_auc(ties,binary,2,&result) && result==.5);
    assert(roc_auc(reversed,binary,2,&result) && result==0);
    assert(roc_auc(perfect,binary,2,&result) && result==1);
    result=-9;
    assert(!roc_auc(perfect,same,2,&result));
    assert(!roc_auc(perfect,invalid,2,&result));
    assert(!roc_auc(bad,binary,2,&result));
    assert(!roc_auc(scores,labels,0,&result));
    assert(!roc_auc(scores,labels,4,NULL));
    assert(result==-9);
    puts("C ROC AUC tests passed");
    return 0;
}
