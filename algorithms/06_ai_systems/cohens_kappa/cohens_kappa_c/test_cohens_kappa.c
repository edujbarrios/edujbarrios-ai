#include "cohens_kappa.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint8_t a[]={0,0,1,1},b[]={0,0,0,1};
    const uint8_t perfect[]={0,1},opposite[]={1,0};
    const uint8_t zeros[]={0,0},ones[]={1,1};
    const uint8_t bad[]={0,2};
    const uint8_t chance_a[]={0,0,1,1},chance_b[]={0,1,0,1};
    double kappa=-9;
    assert(cohens_kappa(a,b,4,&kappa)&&fabs(kappa-.5)<1e-12);
    assert(cohens_kappa(perfect,opposite,2,&kappa)&&fabs(kappa+1)<1e-12);
    assert(cohens_kappa(perfect,perfect,2,&kappa)&&kappa==1);
    assert(cohens_kappa(chance_a,chance_b,4,&kappa)&&kappa==0);
    assert(cohens_kappa(zeros,ones,2,&kappa)&&kappa==0);
    kappa=-9;
    assert(!cohens_kappa(zeros,zeros,2,&kappa));
    assert(!cohens_kappa(ones,ones,2,&kappa));
    assert(!cohens_kappa(zeros,bad,2,&kappa));
    assert(!cohens_kappa(perfect,perfect,0,&kappa));
    assert(!cohens_kappa(perfect,perfect,2,NULL));
    assert(kappa==-9);
    puts("C Cohen kappa tests passed");
    return 0;
}
