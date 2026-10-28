#include "topological_sort.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const int dag[]={
        0,1,1,0,
        0,0,0,1,
        0,0,0,1,
        0,0,0,0
    };
    const int independent[]={0,0,0,0,0,0,0,0,0};
    const int tie[]={0,0,0,0,0,0,1,0,0};
    const int cycle[]={0,1,1,0};
    const int single_cycle[]={1};
    const int single[]={0};
    const int bad[]={0,2,0,0},negative[]={0,-1,0,0};
    size_t out[]={99,99,99,99};
    assert(topological_sort(dag,4,out));
    assert(out[0]==0 && out[1]==1 && out[2]==2 && out[3]==3);
    assert(topological_sort(independent,3,out));
    assert(out[0]==0 && out[1]==1 && out[2]==2);
    assert(topological_sort(tie,3,out));
    assert(out[0]==1 && out[1]==2 && out[2]==0);
    assert(topological_sort(single,1,out) && out[0]==0);
    out[0]=99;
    assert(!topological_sort(cycle,2,out));
    assert(!topological_sort(single_cycle,1,out));
    assert(!topological_sort(dag,0,out));
    assert(!topological_sort(dag,129,out));
    assert(!topological_sort(bad,2,out));
    assert(!topological_sort(negative,2,out));
    assert(!topological_sort(NULL,1,out));
    assert(!topological_sort(dag,4,NULL));
    assert(out[0]==99);
    puts("C topological sort tests passed");
    return 0;
}
