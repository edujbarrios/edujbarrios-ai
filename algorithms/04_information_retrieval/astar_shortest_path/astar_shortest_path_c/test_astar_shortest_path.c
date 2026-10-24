#include "astar_shortest_path.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double graph[]={0,2,5,0, 0,0,1,10, 0,0,0,2, 0,0,0,0};
    const double h[]={4,3,2,0};
    const double zero[]={0,0,0,0};
    const double disconnected[]={0,1,0, 0,0,0, 0,0,0};
    const double negative[]={0,-1,0, 0,0,0, 0,0,0};
    const double h3[]={0,0,0},invalid_h[]={2,0,0};
    const double inconsistent[]={10,3,2,0};
    double length=-9;
    size_t path[4]={99,99,99,99},count=99;
    assert(astar_shortest_path(graph,h,4,0,3,&length,path,&count));
    assert(length==5 && count==4 && path[0]==0 && path[1]==1 &&
           path[2]==2 && path[3]==3);
    assert(astar_shortest_path(graph,zero,4,0,3,&length,path,&count));
    assert(length==5 && count==4);
    assert(astar_shortest_path(graph,h,4,3,3,&length,path,&count));
    assert(length==0 && count==1 && path[0]==3);
    assert(astar_shortest_path(disconnected,h3,3,0,2,&length,path,&count));
    assert(length==-1 && count==0);
    length=-9;count=99;path[0]=99;
    assert(!astar_shortest_path(graph,inconsistent,4,0,3,&length,path,&count));
    assert(!astar_shortest_path(graph,h,4,0,2,&length,path,&count));
    assert(!astar_shortest_path(negative,h3,3,0,2,&length,path,&count));
    assert(!astar_shortest_path(disconnected,invalid_h,3,0,2,&length,path,&count));
    assert(!astar_shortest_path(graph,h,4,4,3,&length,path,&count));
    assert(!astar_shortest_path(graph,h,0,0,0,&length,path,&count));
    assert(length==-9 && count==99 && path[0]==99);
    puts("C A-star tests passed");
    return 0;
}
