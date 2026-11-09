#include "undirected_graph_bridges.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void edge(uint8_t *a, size_t n, size_t u, size_t v) {
    a[u*n+v]=a[v*n+u]=1;
}
static void check(const uint8_t *a, size_t n, const size_t *expected, size_t pairs) {
    size_t out[4032]={0}, count=999;
    assert(undirected_graph_bridges(a,n,out,&count));
    assert(count==pairs);
    for (size_t i=0;i<2*pairs;++i) assert(out[i]==expected[i]);
}
int main(void) {
    uint8_t a[64*64]={0};
    const size_t path[]={0,1,1,2,2,3},star[]={0,1,0,2,0,3};
    const size_t tail[]={2,3,3,4},disconnected[]={0,1,1,2};
    for(size_t i=0;i<3;++i) edge(a,4,i,i+1);
    check(a,4,path,3);
    memset(a,0,sizeof(a));
    edge(a,4,0,1);edge(a,4,1,2);edge(a,4,2,3);edge(a,4,3,0);
    check(a,4,NULL,0);
    memset(a,0,sizeof(a));
    for(size_t i=1;i<4;++i)edge(a,4,0,i);
    check(a,4,star,3);
    memset(a,0,sizeof(a));
    edge(a,5,0,1);edge(a,5,1,2);edge(a,5,2,0);
    edge(a,5,2,3);edge(a,5,3,4);
    check(a,5,tail,2);
    memset(a,0,sizeof(a));
    edge(a,7,0,1);edge(a,7,1,2);
    edge(a,7,3,4);edge(a,7,4,5);edge(a,7,5,3);
    check(a,7,disconnected,2);
    memset(a,0,sizeof(a));
    for(size_t i=0;i<4;++i)
        for(size_t j=i+1;j<4;++j)edge(a,4,i,j);
    check(a,4,NULL,0);
    memset(a,0,sizeof(a));
    check(a,1,NULL,0);
    edge(a,2,0,1);
    const size_t one_edge[]={0,1};
    check(a,2,one_edge,1);
    memset(a,0,sizeof(a));
    for(size_t i=0;i<63;++i)edge(a,64,i,i+1);
    size_t expected[126];
    for(size_t i=0;i<63;++i){expected[2*i]=i;expected[2*i+1]=i+1;}
    check(a,64,expected,63);
    size_t out[4032]={777},count=888;
    const uint8_t selfloop[]={1},asym[]={0,1,0,0},bad[]={0,2,2,0};
    assert(!undirected_graph_bridges(a,0,out,&count));
    assert(!undirected_graph_bridges(a,65,out,&count));
    assert(!undirected_graph_bridges(NULL,1,out,&count));
    assert(!undirected_graph_bridges(a,1,NULL,&count));
    assert(!undirected_graph_bridges(a,1,out,NULL));
    assert(!undirected_graph_bridges(a,1,out,out));
    assert(!undirected_graph_bridges(selfloop,1,out,&count));
    assert(!undirected_graph_bridges(asym,2,out,&count));
    assert(!undirected_graph_bridges(bad,2,out,&count));
    assert(out[0]==777 && count==888);
    puts("C undirected bridge tests passed");
    return 0;
}
