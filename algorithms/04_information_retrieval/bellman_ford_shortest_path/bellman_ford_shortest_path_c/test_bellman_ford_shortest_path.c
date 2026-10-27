#include "bellman_ford_shortest_path.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const BFEdge graph[] = {{0,1,4},{0,2,5},{1,2,-2},{2,3,3}};
    const BFEdge zeros[] = {{0,1,0},{0,1,5},{1,2,-1}};
    const BFEdge negcycle[] = {{0,1,1},{1,2,-3},{2,1,1}};
    const BFEdge remote[] = {{2,3,-3},{3,2,1}};
    const BFEdge bad_vertex[] = {{0,3,1}};
    const BFEdge nonfinite[] = {{0,1,NAN}};
    const BFEdge excessive[] = {{0,1,1000001}};
    const BFEdge self_cycle[] = {{0,0,-1}};
    double out[] = {99,99,99,99,99};
    assert(bellman_ford_shortest_path(graph,4,5,0,out));
    assert(out[0]==0 && out[1]==4 && out[2]==2 && out[3]==5 && isinf(out[4]));
    assert(bellman_ford_shortest_path(graph,4,5,4,out));
    assert(isinf(out[0]) && out[4]==0);
    assert(bellman_ford_shortest_path(zeros,3,3,0,out));
    assert(out[0]==0 && out[1]==0 && out[2]==-1);
    assert(bellman_ford_shortest_path(remote,2,4,0,out));
    assert(out[0]==0 && isinf(out[2]) && isinf(out[3]));
    assert(bellman_ford_shortest_path(NULL,0,1,0,out) && out[0]==0);
    out[0]=99;
    assert(!bellman_ford_shortest_path(negcycle,3,3,0,out));
    assert(!bellman_ford_shortest_path(self_cycle,1,1,0,out));
    assert(!bellman_ford_shortest_path(bad_vertex,1,3,0,out));
    assert(!bellman_ford_shortest_path(nonfinite,1,3,0,out));
    assert(!bellman_ford_shortest_path(excessive,1,3,0,out));
    assert(!bellman_ford_shortest_path(NULL,1,3,0,out));
    assert(!bellman_ford_shortest_path(graph,4,0,0,out));
    assert(!bellman_ford_shortest_path(graph,4,5,5,out));
    assert(!bellman_ford_shortest_path(graph,16385,5,0,out));
    assert(!bellman_ford_shortest_path(graph,4,5,0,NULL));
    assert(out[0]==99);
    puts("C Bellman-Ford tests passed");
    return 0;
}
