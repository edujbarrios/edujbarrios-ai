#include "perplexity.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double logs[]={-0.6931471805599453,-1.3862943611198906};
    const double equal[]={-1.3862943611198906,-1.3862943611198906};
    const double certain[]={0,0,0};
    const double invalid[]={NAN},positive[]={.1},impossible[]={-INFINITY};
    const double extreme[]={-1000};
    double result=-9;
    assert(perplexity(logs,2,&result));
    assert(fabs(result-sqrt(8.0))<1e-12);
    assert(perplexity(equal,2,&result) && fabs(result-4)<1e-12);
    assert(perplexity(certain,3,&result)&&result==1.0);
    assert(perplexity(logs,1,&result)&&fabs(result-2)<1e-12);
    result=-9;
    assert(!perplexity(invalid,1,&result));
    assert(!perplexity(positive,1,&result));
    assert(!perplexity(impossible,1,&result));
    assert(!perplexity(extreme,1,&result));
    assert(!perplexity(logs,0,&result));
    assert(!perplexity(logs,2,NULL));
    assert(!perplexity(NULL,2,&result));
    assert(result==-9);
    puts("C perplexity tests passed");
    return 0;
}
