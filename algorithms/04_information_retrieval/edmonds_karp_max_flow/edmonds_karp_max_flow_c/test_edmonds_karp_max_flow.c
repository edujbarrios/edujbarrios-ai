#include "edmonds_karp_max_flow.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void add(uint16_t *a,size_t n,size_t u,size_t v,uint16_t c) {
    a[u*n+v]=c;
}
static void check(const uint16_t *a,size_t n,size_t s,size_t t,uint32_t want) {
    uint32_t actual=999999;
    assert(edmonds_karp_max_flow(a,n,s,t,&actual));
    assert(actual==want);
}
int main(void) {
    uint16_t a[256]={0};
    add(a,6,0,1,16);add(a,6,0,2,13);
    add(a,6,1,2,10);add(a,6,2,1,4);
    add(a,6,1,3,12);add(a,6,2,4,14);
    add(a,6,3,2,9);add(a,6,4,3,7);
    add(a,6,3,5,20);add(a,6,4,5,4);
    check(a,6,0,5,23);
    memset(a,0,sizeof(a));
    add(a,2,0,1,7);
    check(a,2,0,1,7);
    check(a,2,1,0,0);
    memset(a,0,sizeof(a));
    add(a,3,0,1,4);add(a,3,1,0,3);
    add(a,3,1,2,2);add(a,3,0,2,1);
    check(a,3,0,2,3);
    memset(a,0,sizeof(a));
    add(a,4,0,1,6);add(a,4,2,3,7);
    check(a,4,0,3,0);
    memset(a,0,sizeof(a));
    check(a,3,0,2,0);
    for(size_t i=1;i<15;++i) { add(a,16,0,i,1000);add(a,16,i,15,1000); }
    check(a,16,0,15,14000);
    uint32_t out=777;
    const uint16_t bad[]={0,1001,0,0};
    const uint16_t loop[]={1,0,0,0};
    assert(!edmonds_karp_max_flow(NULL,2,0,1,&out));
    assert(!edmonds_karp_max_flow(a,1,0,0,&out));
    assert(!edmonds_karp_max_flow(a,17,0,1,&out));
    assert(!edmonds_karp_max_flow(a,2,0,0,&out));
    assert(!edmonds_karp_max_flow(a,2,0,2,&out));
    assert(!edmonds_karp_max_flow(a,2,2,1,&out));
    assert(!edmonds_karp_max_flow(a,2,0,1,NULL));
    assert(!edmonds_karp_max_flow(bad,2,0,1,&out));
    assert(!edmonds_karp_max_flow(loop,2,0,1,&out));
    assert(out==777);
    puts("C max flow tests passed");
    return 0;
}
