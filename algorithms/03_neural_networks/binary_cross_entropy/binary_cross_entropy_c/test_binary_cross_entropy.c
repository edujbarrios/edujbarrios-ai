#include "binary_cross_entropy.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void){
 double y[]={1,0},p[]={1,0},out=-1;
 assert(binary_cross_entropy(y,p,2,&out)&&out==0);
 p[0]=0.5;p[1]=0.5;
 assert(binary_cross_entropy(y,p,2,&out)&&fabs(out-log(2))<1e-12);
 out=-1;p[0]=0;
 assert(!binary_cross_entropy(y,p,2,&out)&&out==-1);
 y[0]=2;p[0]=0.5;
 assert(!binary_cross_entropy(y,p,2,&out));
 puts("C BCE tests passed");return 0;
}
