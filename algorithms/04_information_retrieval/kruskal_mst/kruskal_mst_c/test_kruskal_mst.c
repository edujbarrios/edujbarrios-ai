#include "kruskal_mst.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double graph[]={0,1,4,INFINITY,1,0,2,5,4,2,0,3,INFINITY,5,3,0};
    const double tie[]={0,1,1,1,0,1,1,1,0};
    const double zero[]={0,0,2,0,0,1,2,1,0};
    const double disconnected[]={0,INFINITY,INFINITY,0};
    const double asymmetric[]={0,1,2,0};
    const double negative[]={0,-1,-1,0};
    const double nan[]={0,NAN,NAN,0};
    const double bad_diagonal[]={1,1,1,0};
    size_t u[3]={99,99,99},v[3]={99,99,99};
    double cost=-9;
    assert(kruskal_mst(graph,4,u,v,&cost));
    assert(cost==6 && u[0]==0 && v[0]==1 &&
           u[1]==1 && v[1]==2 && u[2]==2 && v[2]==3);
    assert(kruskal_mst(tie,3,u,v,&cost));
    assert(cost==2 && u[0]==0 && v[0]==1 && u[1]==0 && v[1]==2);
    assert(kruskal_mst(zero,3,u,v,&cost));
    assert(cost==1 && u[0]==0 && v[0]==1);
    assert(kruskal_mst((const double[]){0},1,NULL,NULL,&cost)&&cost==0);
    u[0]=99;v[0]=99;cost=-9;
    assert(!kruskal_mst(disconnected,2,u,v,&cost));
    assert(!kruskal_mst(asymmetric,2,u,v,&cost));
    assert(!kruskal_mst(negative,2,u,v,&cost));
    assert(!kruskal_mst(nan,2,u,v,&cost));
    assert(!kruskal_mst(bad_diagonal,2,u,v,&cost));
    assert(!kruskal_mst(graph,0,u,v,&cost));
    assert(!kruskal_mst(graph,4,u,v,NULL));
    assert(u[0]==99 && v[0]==99 && cost==-9);
    puts("C Kruskal MST tests passed");
    return 0;
}
