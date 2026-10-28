#include "cosine_annealing_learning_rate.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    double out=99;
    assert(cosine_annealing_learning_rate(1,0,0,4,&out) && out==1);
    assert(cosine_annealing_learning_rate(1,0,4,4,&out) && out==0);
    assert(cosine_annealing_learning_rate(1,0,2,4,&out) &&
           fabs(out-0.5)<1e-12);
    assert(cosine_annealing_learning_rate(1,0,1,4,&out) &&
           fabs(out-0.8535533905932737)<1e-12);
    assert(cosine_annealing_learning_rate(1,0,3,4,&out) &&
           fabs(out-0.14644660940672627)<1e-12);
    assert(cosine_annealing_learning_rate(0.5,0.5,1,3,&out) && out==0.5);
    assert(cosine_annealing_learning_rate(1000000,0,1,1,&out) && out==0);
    out=99;
    assert(!cosine_annealing_learning_rate(0,1,1,4,&out));
    assert(!cosine_annealing_learning_rate(1,-1,1,4,&out));
    assert(!cosine_annealing_learning_rate(1000001,0,1,4,&out));
    assert(!cosine_annealing_learning_rate(NAN,0,1,4,&out));
    assert(!cosine_annealing_learning_rate(INFINITY,0,1,4,&out));
    assert(!cosine_annealing_learning_rate(1,0,1,0,&out));
    assert(!cosine_annealing_learning_rate(1,0,5,4,&out));
    assert(!cosine_annealing_learning_rate(1,0,1,100001,&out));
    assert(!cosine_annealing_learning_rate(1,0,1,4,NULL));
    assert(out==99);
    puts("C cosine annealing tests passed");
    return 0;
}
