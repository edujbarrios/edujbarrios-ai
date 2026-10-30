#include "logit_bias_adjustment.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double logits[] = {1,2,3};
    const size_t ids[] = {0,2}, dup[] = {1,1}, invalid[] = {3};
    const double biases[] = {-1,0.5}, bad[] = {NAN,0.5};
    double output[] = {9,9,9};
    assert(logit_bias_adjustment(logits,3,ids,biases,2,output));
    assert(output[0]==0 && output[1]==2 && output[2]==3.5);
    assert(logit_bias_adjustment(logits,3,NULL,NULL,0,output));
    assert(output[0]==1 && output[1]==2 && output[2]==3);
    double inplace[] = {1,2,3};
    assert(logit_bias_adjustment(inplace,3,ids,biases,2,inplace));
    assert(inplace[0]==0 && inplace[2]==3.5);
    output[0]=9;
    assert(!logit_bias_adjustment(logits,3,dup,biases,2,output) && output[0]==9);
    assert(!logit_bias_adjustment(logits,3,invalid,biases,1,output) && output[0]==9);
    assert(!logit_bias_adjustment(logits,3,ids,bad,2,output) && output[0]==9);
    assert(!logit_bias_adjustment(logits,0,NULL,NULL,0,output) && output[0]==9);
    assert(!logit_bias_adjustment(NULL,3,ids,biases,2,output) && output[0]==9);
    assert(!logit_bias_adjustment(logits,3,ids,biases,2,NULL));
    puts("C sparse logit bias tests passed");
    return 0;
}
