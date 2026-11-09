#include "articulation_points.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void edge(uint8_t *a,size_t n,size_t u,size_t v) {
    a[u*n+v]=a[v*n+u]=1;
}
static void check(const uint8_t *a,size_t n,const size_t *want,size_t k) {
    size_t out[64]={0},count=999;
    assert(articulation_points(a,n,out,&count));
    assert(count==k);
    for(size_t i=0;i<k;++i) assert(out[i]==want[i]);
}
int main(void) {
    uint8_t a[64*64]={0};
    const size_t path[]={1,2,3}, star[]={0}, disconnected[]={1};
    for(size_t i=0;i<4;++i) edge(a,5,i,i+1);
    check(a,5,path,3);
    memset(a,0,sizeof(a));
    edge(a,4,0,1);edge(a,4,1,2);edge(a,4,2,3);edge(a,4,3,0);
    check(a,4,NULL,0);
    memset(a,0,sizeof(a));
    for(size_t i=1;i<5;++i) edge(a,5,0,i);
    check(a,5,star,1);
    memset(a,0,sizeof(a));
    edge(a,6,0,1);edge(a,6,1,2);edge(a,6,3,4);
    check(a,6,disconnected,1);
    memset(a,0,sizeof(a));
    for(size_t u=0;u<4;++u)for(size_t v=u+1;v<4;++v)edge(a,4,u,v);
    check(a,4,NULL,0);
    memset(a,0,sizeof(a));
    check(a,1,NULL,0);
    edge(a,2,0,1);check(a,2,NULL,0);
    memset(a,0,sizeof(a));
    for(size_t i=0;i<63;++i)edge(a,64,i,i+1);
    size_t want[62];
    for(size_t i=0;i<62;++i)want[i]=i+1;
    check(a,64,want,62);

    size_t out[64]={777},count=888;
    uint8_t selfloop[]={1}, asymmetric[]={0,1,0,0}, nonbinary[]={0,2,2,0};
    assert(!articulation_points(a,0,out,&count));
    assert(!articulation_points(a,65,out,&count));
    assert(!articulation_points(NULL,1,out,&count));
    assert(!articulation_points(a,1,NULL,&count));
    assert(!articulation_points(a,1,out,NULL));
    assert(!articulation_points(a,1,out,out));
    assert(!articulation_points(selfloop,1,out,&count));
    assert(!articulation_points(asymmetric,2,out,&count));
    assert(!articulation_points(nonbinary,2,out,&count));
    assert(out[0]==777 && count==888);
    puts("C articulation points tests passed");
    return 0;
}
