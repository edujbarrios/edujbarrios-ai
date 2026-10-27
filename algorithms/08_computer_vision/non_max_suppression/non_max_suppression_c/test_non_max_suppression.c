#include "non_max_suppression.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const NMSBox boxes[] = {
        {0,0,2,2,0.9},{0.5,0.5,2.5,2.5,0.8},
        {3,3,5,5,0.7},{0,0,2,2,0.9},{10,10,12,12,0.7}};
    const NMSBox touching[]={{0,0,1,1,1},{1,0,2,1,0.8}};
    const NMSBox malformed[]={{0,0,0,2,0.5}};
    const NMSBox invalid[]={{0,0,1,1,NAN}};
    const NMSBox overflow[]={{0,0,1000001,1,0.5}};
    size_t indices[5]={99,99,99,99,99}, count=99;
    assert(non_max_suppression(boxes,5,0.3,indices,&count));
    assert(count==3 && indices[0]==0 && indices[1]==2 && indices[2]==4);
    assert(non_max_suppression(boxes,5,0.4,indices,&count));
    assert(count==4 && indices[0]==0 && indices[1]==1 &&
           indices[2]==2 && indices[3]==4);
    assert(non_max_suppression(touching,2,0,indices,&count) && count==2);
    assert(non_max_suppression(NULL,0,1,NULL,&count) && count==0);
    count=99;indices[0]=99;
    assert(!non_max_suppression(malformed,1,0.5,indices,&count));
    assert(!non_max_suppression(invalid,1,0.5,indices,&count));
    assert(!non_max_suppression(overflow,1,0.5,indices,&count));
    assert(!non_max_suppression(boxes,5,-0.1,indices,&count));
    assert(!non_max_suppression(boxes,5,INFINITY,indices,&count));
    assert(!non_max_suppression(boxes,257,0.5,indices,&count));
    assert(!non_max_suppression(NULL,1,0.5,indices,&count));
    assert(!non_max_suppression(boxes,5,0.5,NULL,&count));
    assert(!non_max_suppression(boxes,5,0.5,indices,NULL));
    assert(count==99 && indices[0]==99);
    puts("C non-maximum suppression tests passed");
    return 0;
}
