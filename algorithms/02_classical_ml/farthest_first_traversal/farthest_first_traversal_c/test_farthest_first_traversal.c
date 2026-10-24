#include "farthest_first_traversal.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double points[]={0,0,1,0,9,0,10,0};
    const double repeats[]={0,0,0,0,2,0,2,0};
    const double one[]={0};
    const double invalid[]={0,0,INFINITY,0};
    const double extremes[]={1.7e308,-1.7e308};
    size_t selected[]={99,99,99,99};
    assert(farthest_first_traversal(points,4,2,3,selected));
    assert(selected[0]==0 && selected[1]==3 && selected[2]==1);
    assert(farthest_first_traversal(points,4,2,4,selected));
    assert(selected[0]==0 && selected[1]==3 && selected[2]==1 && selected[3]==2);
    assert(farthest_first_traversal(repeats,4,2,3,selected));
    assert(selected[0]==0 && selected[1]==2 && selected[2]==1);
    assert(farthest_first_traversal(one,1,1,1,selected)&&selected[0]==0);
    selected[0]=99;
    assert(!farthest_first_traversal(points,4,2,0,selected));
    assert(!farthest_first_traversal(points,4,2,5,selected));
    assert(!farthest_first_traversal(invalid,2,2,1,selected));
    assert(!farthest_first_traversal(extremes,2,1,2,selected));
    assert(!farthest_first_traversal(points,0,2,1,selected));
    assert(!farthest_first_traversal(points,4,2,1,NULL));
    assert(selected[0]==99);
    puts("C farthest-first traversal tests passed");
    return 0;
}
