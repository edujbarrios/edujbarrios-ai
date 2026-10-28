#include "prim_mst.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double graph[] = {
        0,1,4,0,
        1,0,2,5,
        4,2,0,3,
        0,5,3,0
    };
    const double ties[] = {0,1,1, 1,0,1, 1,1,0};
    const double disconnected[] = {0,0,0,0};
    const double asymmetric[] = {0,1,2,0};
    const double bad_diagonal[] = {1,1,1,0};
    const double negative[] = {0,-1,-1,0};
    const double nonfinite[] = {0,NAN,NAN,0};
    const double large[] = {0,1000001,1000001,0};
    const double singleton[] = {0};
    double output = 99.0;
    assert(prim_mst(graph,4,&output) && output==6.0);
    assert(prim_mst(ties,3,&output) && output==2.0);
    assert(prim_mst(singleton,1,&output) && output==0.0);
    output=99.0;
    assert(!prim_mst(disconnected,2,&output));
    assert(!prim_mst(asymmetric,2,&output));
    assert(!prim_mst(bad_diagonal,2,&output));
    assert(!prim_mst(negative,2,&output));
    assert(!prim_mst(nonfinite,2,&output));
    assert(!prim_mst(large,2,&output));
    assert(!prim_mst(graph,0,&output));
    assert(!prim_mst(graph,129,&output));
    assert(!prim_mst(NULL,1,&output));
    assert(!prim_mst(graph,4,NULL));
    assert(output==99.0);
    puts("C Prim MST tests passed");
    return 0;
}
