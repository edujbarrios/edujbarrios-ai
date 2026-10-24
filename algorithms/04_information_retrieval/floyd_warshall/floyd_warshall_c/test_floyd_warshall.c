#include "floyd_warshall.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double edges[]={0,3,INFINITY,INFINITY,0,-2,4,INFINITY,0};
    const double answer[]={0,3,1,2,0,-2,4,7,0};
    const double disconnected[]={0,INFINITY,INFINITY,0};
    const double negative_cycle[]={0,-2,-2,0};
    const double bad_diagonal[]={1,INFINITY,INFINITY,0};
    const double bad_nan[]={0,NAN,INFINITY,0};
    const double bad_neg_inf[]={0,-INFINITY,INFINITY,0};
    const double bad_magnitude[]={0,1000001,INFINITY,0};
    double distances[9]={-9,-9,-9,-9,-9,-9,-9,-9,-9};
    assert(floyd_warshall(edges,3,distances));
    for (size_t i=0;i<9;++i) assert(fabs(distances[i]-answer[i])<1e-12);
    assert(floyd_warshall(disconnected,2,distances));
    assert(isinf(distances[1]) && isinf(distances[2]));
    const double one[]={0};
    assert(floyd_warshall(one,1,distances)&&distances[0]==0);
    distances[0]=-9;
    assert(!floyd_warshall(negative_cycle,2,distances));
    assert(!floyd_warshall(bad_diagonal,2,distances));
    assert(!floyd_warshall(bad_nan,2,distances));
    assert(!floyd_warshall(bad_neg_inf,2,distances));
    assert(!floyd_warshall(bad_magnitude,2,distances));
    assert(!floyd_warshall(one,0,distances));
    assert(!floyd_warshall(one,1,NULL));
    assert(distances[0]==-9);
    puts("C Floyd-Warshall tests passed");
    return 0;
}
