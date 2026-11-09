#include "macro_f1_score.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void check(const uint8_t *a,const uint8_t *p,size_t n,size_t k,double want) {
    double output=-777;
    assert(macro_f1_score(a,p,n,k,&output));
    assert(fabs(output-want)<1e-12);
}
int main(void) {
    const uint8_t a[]={0,1,2,2},p[]={0,2,1,2};
    check(a,p,4,3,0.5);
    const uint8_t zeros[]={0,0},ones[]={1,1};
    check(zeros,zeros,2,3,1.0/3.0);
    check(zeros,ones,2,2,0.0);
    const uint8_t perfect[]={0,1,2};
    check(perfect,perfect,3,3,1.0);
    const uint8_t partial[]={0,0,2};
    check(perfect,partial,3,3,(2.0/3.0+1.0)/3.0);
    check(zeros,zeros,2,2,0.5);
    const uint8_t last[]={15};
    check(last,last,1,16,1.0/16.0);
    uint8_t long_zero[512]={0};
    check(long_zero,long_zero,512,16,1.0/16.0);
    double result=777;
    const uint8_t out_of_range[]={2},neg_encoded[]={255};
    assert(!macro_f1_score(NULL,ones,2,2,&result));
    assert(!macro_f1_score(zeros,NULL,2,2,&result));
    assert(!macro_f1_score(zeros,ones,0,2,&result));
    assert(!macro_f1_score(zeros,ones,513,2,&result));
    assert(!macro_f1_score(zeros,ones,2,1,&result));
    assert(!macro_f1_score(zeros,ones,2,17,&result));
    assert(!macro_f1_score(out_of_range,ones,1,2,&result));
    assert(!macro_f1_score(zeros,out_of_range,1,2,&result));
    assert(!macro_f1_score(neg_encoded,ones,1,2,&result));
    assert(!macro_f1_score(zeros,ones,2,2,NULL));
    assert(result==777);
    puts("C macro F1 tests passed");
    return 0;
}
