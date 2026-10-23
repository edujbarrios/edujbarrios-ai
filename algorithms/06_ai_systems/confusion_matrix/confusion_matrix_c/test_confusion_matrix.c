#include "confusion_matrix.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint8_t actual[]={0,0,1,1,1};
    const uint8_t predicted[]={0,1,1,0,1};
    const uint8_t perfect[]={0,1};
    const uint8_t zeros[]={0,0};
    const uint8_t bad[]={0,2};
    size_t out[4]={99,99,99,99};
    assert(confusion_matrix(actual,predicted,5,out));
    assert(out[0]==1 && out[1]==1 && out[2]==1 && out[3]==2);
    assert(confusion_matrix(perfect,perfect,2,out));
    assert(out[0]==1 && out[1]==0 && out[2]==0 && out[3]==1);
    assert(confusion_matrix(zeros,zeros,2,out));
    assert(out[0]==2 && out[1]==0 && out[2]==0 && out[3]==0);
    out[0]=99;
    assert(!confusion_matrix(actual,predicted,0,out));
    assert(!confusion_matrix(perfect,bad,2,out));
    assert(!confusion_matrix(NULL,predicted,5,out));
    assert(!confusion_matrix(actual,predicted,5,NULL));
    assert(out[0]==99);
    puts("C confusion matrix tests passed");
    return 0;
}
