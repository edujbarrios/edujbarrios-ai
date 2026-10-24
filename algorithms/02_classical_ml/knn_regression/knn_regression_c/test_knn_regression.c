#include "knn_regression.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double points[]={0,0,2,0,0,2,5,5};
    const double targets[]={2,4,6,10};
    const double query[]={0,0};
    const double duplicates[]={0,0,0,0};
    const double dup_targets[]={-5,7};
    const double invalid[]={0,0,0,NAN};
    const double extremes[]={1.7e308,-1.7e308};
    const double extreme_q[]={-1.7e308};
    const double huge_targets[]={1e308,1e308};
    double prediction=-9;
    assert(knn_regression(points,targets,4,2,query,1,&prediction)&&prediction==2);
    assert(knn_regression(points,targets,4,2,query,2,&prediction)&&prediction==3);
    assert(knn_regression(points,targets,4,2,query,3,&prediction)&&prediction==4);
    assert(knn_regression(points,targets,4,2,query,4,&prediction)&&prediction==5.5);
    assert(knn_regression(duplicates,dup_targets,2,2,query,2,&prediction));
    assert(prediction==1);
    assert(knn_regression(points,targets,1,2,query,1,&prediction)&&prediction==2);
    assert(knn_regression(duplicates,huge_targets,2,2,query,2,&prediction));
    assert(isfinite(prediction) && fabs(prediction/1e308-1)<1e-12);
    prediction=-9;
    assert(!knn_regression(points,targets,4,2,query,0,&prediction));
    assert(!knn_regression(points,targets,4,2,query,5,&prediction));
    assert(!knn_regression(invalid,dup_targets,2,2,query,1,&prediction));
    assert(!knn_regression(extremes,dup_targets,2,1,extreme_q,1,&prediction));
    assert(!knn_regression(points,targets,4,0,query,1,&prediction));
    assert(!knn_regression(points,targets,4,2,query,1,NULL));
    assert(prediction==-9);
    puts("C KNN regression tests passed");
    return 0;
}
