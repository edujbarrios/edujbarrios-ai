#include "undirected_connected_components.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const uint8_t graph[]={
        0,0,1,0,0,0,
        0,0,0,0,1,0,
        1,0,0,1,0,0,
        0,0,1,0,0,0,
        0,1,0,0,0,0,
        0,0,0,0,0,0
    };
    const size_t expect[]={0,1,0,0,1,5};
    const uint8_t cycle[]={1,1,1,0};
    const uint8_t none[]={0,0,0,0},self[]={1};
    const uint8_t asym[]={0,1,0,0},bad[]={0,2,2,0};
    size_t out[]={99,99,99,99,99,99};
    assert(undirected_connected_components(graph,6,out));
    for(size_t i=0;i<6;++i) assert(out[i]==expect[i]);
    assert(undirected_connected_components(cycle,2,out));
    assert(out[0]==0 && out[1]==0);
    assert(undirected_connected_components(none,2,out));
    assert(out[0]==0 && out[1]==1);
    assert(undirected_connected_components(self,1,out) && out[0]==0);
    out[0]=99;
    assert(!undirected_connected_components(asym,2,out));
    assert(!undirected_connected_components(bad,2,out));
    assert(!undirected_connected_components(graph,0,out));
    assert(!undirected_connected_components(graph,129,out));
    assert(!undirected_connected_components(NULL,1,out));
    assert(!undirected_connected_components(graph,6,NULL));
    assert(out[0]==99);
    puts("C undirected connected components tests passed");
    return 0;
}
