#include "exact_cosine_top_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double query[]={1,0};
    const double candidates[]={1,0, 0,1, -1,0, 1,1, 1,0};
    const double zeros[]={1,0, 0,0};
    const double invalid[]={1,0, NAN,1};
    const double large[]={1e308,1e308};
    const double scalar_q[]={1};
    const double scalar_c[]={-2,3,3};
    size_t indices[]={99,99,99,99,99};
    assert(exact_cosine_top_k(query,2,candidates,5,4,indices));
    assert(indices[0]==0 && indices[1]==4 &&
           indices[2]==3 && indices[3]==1);
    assert(exact_cosine_top_k(scalar_q,1,scalar_c,3,3,indices));
    assert(indices[0]==1 && indices[1]==2 && indices[2]==0);
    indices[0]=99;
    assert(!exact_cosine_top_k(query,2,zeros,2,1,indices));
    assert(!exact_cosine_top_k(query,2,invalid,2,1,indices));
    assert(!exact_cosine_top_k(large,2,candidates,5,1,indices));
    assert(!exact_cosine_top_k(query,2,candidates,0,1,indices));
    assert(!exact_cosine_top_k(query,2,candidates,5,0,indices));
    assert(!exact_cosine_top_k(query,2,candidates,5,6,indices));
    assert(!exact_cosine_top_k(query,2,candidates,5,1,NULL));
    assert(indices[0]==99);
    puts("C exact_cosine_top_k tests passed");
    return 0;
}
