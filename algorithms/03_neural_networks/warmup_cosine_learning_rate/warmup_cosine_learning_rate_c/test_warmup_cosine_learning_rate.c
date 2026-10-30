#include "warmup_cosine_learning_rate.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
static void close_rate(size_t step, double expected) {
    double value = -1;
    assert(warmup_cosine_learning_rate(1,0,2,6,step,&value));
    assert(fabs(value-expected)<1e-12);
}
int main(void) {
    close_rate(0,0); close_rate(1,0.5); close_rate(2,1);
    close_rate(3,0.8535533905932737); close_rate(4,0.5);
    close_rate(5,0.1464466094067262); close_rate(6,0);
    double out = -1;
    assert(warmup_cosine_learning_rate(2,2,1,2,1,&out) && out==2);
    assert(warmup_cosine_learning_rate(2,2,1,2,2,&out) && out==2);
    out=-1;
    assert(!warmup_cosine_learning_rate(1,0,0,6,1,&out) && out==-1);
    assert(!warmup_cosine_learning_rate(1,0,6,6,1,&out) && out==-1);
    assert(!warmup_cosine_learning_rate(1,0,2,6,7,&out) && out==-1);
    assert(!warmup_cosine_learning_rate(0,1,2,6,1,&out) && out==-1);
    assert(!warmup_cosine_learning_rate(INFINITY,0,2,6,1,&out) && out==-1);
    assert(!warmup_cosine_learning_rate(1,0,2,100001,1,&out) && out==-1);
    assert(!warmup_cosine_learning_rate(1,0,2,6,1,NULL));
    puts("C warmup cosine tests passed");
    return 0;
}
