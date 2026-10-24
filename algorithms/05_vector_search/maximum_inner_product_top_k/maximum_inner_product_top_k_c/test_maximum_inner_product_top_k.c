#include "maximum_inner_product_top_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double q[]={1,2};
    const double rows[]={1,0,0,1,1,2,-1,0,1,2};
    const double zeros[]={0,0};
    const double nan_rows[]={0,0,NAN,1};
    const double massive[]={1e308,1e308};
    const double huge_q[]={1e308};
    const double huge_row[]={1e308};
    size_t ids[]={99,99,99,99,99};
    assert(maximum_inner_product_top_k(q,2,rows,5,5,ids));
    assert(ids[0]==2 && ids[1]==4 && ids[2]==1 && ids[3]==0 && ids[4]==3);
    assert(maximum_inner_product_top_k(zeros,2,rows,5,3,ids));
    assert(ids[0]==0 && ids[1]==1 && ids[2]==2);
    ids[0]=99;
    assert(!maximum_inner_product_top_k(q,2,nan_rows,2,1,ids));
    assert(!maximum_inner_product_top_k(huge_q,1,huge_row,1,1,ids));
    assert(!maximum_inner_product_top_k(massive,2,massive,1,1,ids));
    assert(!maximum_inner_product_top_k(q,2,rows,5,0,ids));
    assert(!maximum_inner_product_top_k(q,2,rows,5,6,ids));
    assert(!maximum_inner_product_top_k(q,0,rows,5,1,ids));
    assert(!maximum_inner_product_top_k(q,2,rows,5,1,NULL));
    assert(ids[0]==99);
    puts("C maximum inner-product top-K tests passed");
    return 0;
}
