#include "dbscan.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double points[]={0,0,.1,0,0,.1,5,5,5.1,5,9,9};
    const double border[]={0,1,2,3};
    const double same[]={1,1,1};
    const double far[]={0,10,20};
    const double invalid[]={0,NAN};
    const double extremes[]={-1.7e308,1.7e308};
    int labels[]={99,99,99,99,99,99};
    assert(dbscan(points,6,2,.2,2,labels));
    assert(labels[0]==0 && labels[1]==0 && labels[2]==0 &&
           labels[3]==1 && labels[4]==1 && labels[5]==-1);
    assert(dbscan(border,4,1,1.1,3,labels));
    assert(labels[0]==0 && labels[1]==0 && labels[2]==0 && labels[3]==0);
    assert(dbscan(same,3,1,.1,3,labels));
    assert(labels[0]==0 && labels[1]==0 && labels[2]==0);
    assert(dbscan(far,3,1,1,2,labels));
    assert(labels[0]==-1 && labels[1]==-1 && labels[2]==-1);
    assert(dbscan(far,1,1,1,1,labels) && labels[0]==0);
    labels[0]=99;
    assert(!dbscan(points,6,2,0,2,labels));
    assert(!dbscan(points,6,2,.2,0,labels));
    assert(!dbscan(points,6,2,.2,7,labels));
    assert(!dbscan(invalid,2,1,1,1,labels));
    assert(!dbscan(extremes,2,1,1,1,labels));
    assert(!dbscan(points,0,2,.2,2,labels));
    assert(!dbscan(points,6,2,.2,2,NULL));
    assert(labels[0]==99);
    puts("C DBSCAN tests passed");
    return 0;
}
