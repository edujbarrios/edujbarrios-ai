#include "image_text_recall_at_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double aligned[]={1,0,0,1}, swapped[]={0,1,1,0};
    const double duplicates[]={1,0,1,0}, scaled[]={2,0,0,3};
    const double zeros[]={0,0,0,1}, nan[]={1,0,NAN,1};
    const double excessive[]={1,0,1001,1};
    double result[2]={99,99};
    assert(image_text_recall_at_k(aligned,aligned,2,2,1,result));
    assert(result[0]==1 && result[1]==1);
    assert(image_text_recall_at_k(aligned,swapped,2,2,1,result));
    assert(result[0]==0 && result[1]==0);
    assert(image_text_recall_at_k(aligned,swapped,2,2,2,result));
    assert(result[0]==1 && result[1]==1);
    assert(image_text_recall_at_k(duplicates,duplicates,2,2,1,result));
    assert(result[0]==0.5 && result[1]==0.5);
    assert(image_text_recall_at_k(scaled,aligned,2,2,1,result));
    assert(result[0]==1 && result[1]==1);
    result[0]=99;result[1]=99;
    assert(!image_text_recall_at_k(aligned,aligned,1,2,1,result));
    assert(!image_text_recall_at_k(aligned,aligned,2,0,1,result));
    assert(!image_text_recall_at_k(aligned,aligned,2,129,1,result));
    assert(!image_text_recall_at_k(aligned,aligned,2,2,0,result));
    assert(!image_text_recall_at_k(aligned,aligned,2,2,3,result));
    assert(!image_text_recall_at_k(aligned,zeros,2,2,1,result));
    assert(!image_text_recall_at_k(aligned,nan,2,2,1,result));
    assert(!image_text_recall_at_k(aligned,excessive,2,2,1,result));
    assert(!image_text_recall_at_k(NULL,aligned,2,2,1,result));
    assert(!image_text_recall_at_k(aligned,aligned,2,2,1,NULL));
    assert(result[0]==99 && result[1]==99);
    puts("C image-text retrieval recall tests passed");
    return 0;
}
