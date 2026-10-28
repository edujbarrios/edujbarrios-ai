#include "bipartite_graph_check.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint8_t path[]={0,1,0, 1,0,1, 0,1,0};
    const uint8_t triangle[]={0,1,1, 1,0,1, 1,1,0};
    const uint8_t four[]={0,1,0,1, 1,0,1,0, 0,1,0,1, 1,0,1,0};
    const uint8_t remote_triangle[]={
        0,0,0,0, 0,0,1,1, 0,1,0,1, 0,1,1,0
    };
    const uint8_t isolated[]={0},self[]={1};
    const uint8_t asymmetric[]={0,1,0,0},nonbinary[]={0,2,2,0};
    bool out=false;
    assert(bipartite_graph_check(path,3,&out) && out);
    assert(bipartite_graph_check(four,4,&out) && out);
    assert(bipartite_graph_check(isolated,1,&out) && out);
    assert(bipartite_graph_check(triangle,3,&out) && !out);
    assert(bipartite_graph_check(remote_triangle,4,&out) && !out);
    assert(bipartite_graph_check(self,1,&out) && !out);
    out=true;
    assert(!bipartite_graph_check(asymmetric,2,&out) && out);
    assert(!bipartite_graph_check(nonbinary,2,&out) && out);
    assert(!bipartite_graph_check(path,0,&out) && out);
    assert(!bipartite_graph_check(path,129,&out) && out);
    assert(!bipartite_graph_check(NULL,3,&out) && out);
    assert(!bipartite_graph_check(path,3,NULL) && out);
    puts("C bipartite graph tests passed");
    return 0;
}
