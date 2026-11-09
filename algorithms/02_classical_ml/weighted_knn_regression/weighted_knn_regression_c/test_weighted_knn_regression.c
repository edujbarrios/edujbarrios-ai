#include "weighted_knn_regression.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
static void check(const double *x,const double *y,const double *q,
                  size_t n,size_t d,size_t k,double want) {
    double out=777;
    assert(weighted_knn_regression(x,y,q,n,d,k,&out));
    assert(fabs(out-want)<=1e-11*fmax(1.0,fabs(want)));
}
int main(void){
    const double x[]={0,4},y[]={0,20},q[]={1};
    check(x,y,q,2,1,2,5);
    const double tri[]={0,2,4},ty[]={0,10,20};
    check(tri,ty,q,3,1,2,5);
    check(tri,ty,q,3,1,3,50./7.);
    const double duplicates[]={0,0,2},dy[]={3,7,20},zero[]={0};
    check(duplicates,dy,zero,3,1,3,5);
    check(duplicates,dy,zero,3,1,1,3);
    check(duplicates,dy,zero,3,1,2,5);
    const double tie[]={-1,1},tie_y[]={2,8};
    check(tie,tie_y,zero,2,1,1,2);
    check(tie,tie_y,zero,2,1,2,5);
    const double signed_data[]={0,3,5},signed_y[]={-2,10,20};
    check(signed_data,signed_y,q,3,1,2,2);
    const double single[]={4},single_y[]={12},single_q[]={-3};
    check(single,single_y,single_q,1,1,1,12);
    const double two_d[]={0,0,2,0,0,2},two_y[]={2,4,6},q2[]={1,0};
    check(two_d,two_y,q2,3,2,2,3);
    double big[2048]={0},big_y[128],big_q[16]={0};
    for(size_t i=0;i<128;++i)big_y[i]=7;
    check(big,big_y,big_q,128,16,128,7);
    const double tiny[]={0,0,1e-300},tiny_y[]={5,15,100};
    check(tiny,tiny_y,zero,3,1,3,10);
    double out=777;
    const double invalid[]={NAN},high[]={1001},inf[]={INFINITY};
    assert(!weighted_knn_regression(NULL,y,q,2,1,1,&out));
    assert(!weighted_knn_regression(x,NULL,q,2,1,1,&out));
    assert(!weighted_knn_regression(x,y,NULL,2,1,1,&out));
    assert(!weighted_knn_regression(x,y,q,0,1,1,&out));
    assert(!weighted_knn_regression(x,y,q,129,1,1,&out));
    assert(!weighted_knn_regression(x,y,q,2,0,1,&out));
    assert(!weighted_knn_regression(x,y,q,2,17,1,&out));
    assert(!weighted_knn_regression(x,y,q,2,1,0,&out));
    assert(!weighted_knn_regression(x,y,q,2,1,3,&out));
    assert(!weighted_knn_regression(invalid,y,q,1,1,1,&out));
    assert(!weighted_knn_regression(x,inf,q,1,1,1,&out));
    assert(!weighted_knn_regression(x,y,high,1,1,1,&out));
    assert(!weighted_knn_regression(x,y,q,2,1,1,NULL));
    assert(out==777);
    puts("C weighted kNN regression tests passed");
    return 0;
}
