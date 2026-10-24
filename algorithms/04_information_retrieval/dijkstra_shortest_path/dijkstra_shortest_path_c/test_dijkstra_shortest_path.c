#include "dijkstra_shortest_path.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double w[]={0,2,5,0,0,0,1,0,0,0,0,0,0,0,0,0};
    const double cycle[]={0,1,1,0};
    const double bad[]={0,-1,0,0};
    const double nan[]={0,NAN,0,0};
    const double excessive[]={0,1000001,0,0};
    const double single[]={0};
    double out[]={99,99,99,99};
    assert(dijkstra_shortest_path(w,4,0,out));
    assert(out[0]==0 && out[1]==2 && out[2]==3 && out[3]==-1);
    assert(dijkstra_shortest_path(w,4,3,out));
    assert(out[0]==-1 && out[1]==-1 && out[2]==-1 && out[3]==0);
    assert(dijkstra_shortest_path(cycle,2,0,out));
    assert(out[0]==0 && out[1]==1);
    assert(dijkstra_shortest_path(single,1,0,out) && out[0]==0);
    out[0]=99;
    assert(!dijkstra_shortest_path(w,4,4,out));
    assert(!dijkstra_shortest_path(w,0,0,out));
    assert(!dijkstra_shortest_path(bad,2,0,out));
    assert(!dijkstra_shortest_path(nan,2,0,out));
    assert(!dijkstra_shortest_path(excessive,2,0,out));
    assert(!dijkstra_shortest_path(w,4,0,NULL));
    assert(out[0]==99);
    puts("C Dijkstra tests passed");
    return 0;
}
