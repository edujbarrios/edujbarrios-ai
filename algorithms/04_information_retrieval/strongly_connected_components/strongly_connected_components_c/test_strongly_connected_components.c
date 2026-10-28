#include "strongly_connected_components.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const int graph[] = {
        0,1,0,0,0,0,
        0,0,1,0,0,0,
        1,0,0,1,0,0,
        0,0,0,0,1,0,
        0,0,0,1,0,1,
        0,0,0,0,0,0
    };
    const int cycle[] = {0,1,1,0};
    const int chain[] = {0,1,0,0};
    const int single[] = {1}, isolated[]={0};
    const int bad[] = {0,2,0,0}, negative[]={0,-1,0,0};
    size_t labels[] = {99,99,99,99,99,99};
    const size_t expected[] = {0,0,0,3,3,5};
    assert(strongly_connected_components(graph,6,labels));
    for (size_t i=0;i<6;++i) assert(labels[i]==expected[i]);
    assert(strongly_connected_components(cycle,2,labels));
    assert(labels[0]==0 && labels[1]==0);
    assert(strongly_connected_components(chain,2,labels));
    assert(labels[0]==0 && labels[1]==1);
    assert(strongly_connected_components(single,1,labels) && labels[0]==0);
    assert(strongly_connected_components(isolated,1,labels) && labels[0]==0);
    labels[0]=99;
    assert(!strongly_connected_components(graph,0,labels));
    assert(!strongly_connected_components(graph,129,labels));
    assert(!strongly_connected_components(bad,2,labels));
    assert(!strongly_connected_components(negative,2,labels));
    assert(!strongly_connected_components(NULL,1,labels));
    assert(!strongly_connected_components(graph,6,NULL));
    assert(labels[0]==99);
    puts("C SCC tests passed");
    return 0;
}
