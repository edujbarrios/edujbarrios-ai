#include "bfs_shortest_path.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const int graph[]={
        0,1,1,0,0,0,
        0,0,0,1,0,0,
        0,0,0,1,0,0,
        0,0,0,0,1,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0
    };
    const int cycle[]={1,1,1,0};
    const int singleton[]={1};
    const int bad[]={0,2,0,0};
    const int negative[]={0,-1,0,0};
    const int zero[]={0};
    int out[]={99,99,99,99,99,99};
    assert(bfs_shortest_path(graph,6,0,out));
    const int expected[]={0,1,1,2,3,-1};
    for (size_t i=0;i<6;++i) assert(out[i]==expected[i]);
    assert(bfs_shortest_path(graph,6,5,out));
    for (size_t i=0;i<5;++i) assert(out[i]==-1);
    assert(out[5]==0);
    assert(bfs_shortest_path(cycle,2,0,out) && out[0]==0 && out[1]==1);
    assert(bfs_shortest_path(singleton,1,0,out) && out[0]==0);
    assert(bfs_shortest_path(zero,1,0,out) && out[0]==0);
    out[0]=99;
    assert(!bfs_shortest_path(graph,0,0,out));
    assert(!bfs_shortest_path(graph,6,6,out));
    assert(!bfs_shortest_path(graph,129,0,out));
    assert(!bfs_shortest_path(bad,2,0,out));
    assert(!bfs_shortest_path(negative,2,0,out));
    assert(!bfs_shortest_path(NULL,1,0,out));
    assert(!bfs_shortest_path(graph,6,0,NULL));
    assert(out[0]==99);
    puts("C BFS shortest-path tests passed");
    return 0;
}
