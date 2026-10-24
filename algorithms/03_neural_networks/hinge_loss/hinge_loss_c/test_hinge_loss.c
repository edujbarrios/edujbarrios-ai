#include "hinge_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double scores[]={2,0,-.5,1};
    const int8_t labels[]={1,1,-1,-1};
    const double boundary[]={1,-1};
    const int8_t boundary_labels[]={1,-1};
    const double invalid[]={NAN,1};
    const double extremes[]={-1e308,-1e308};
    const int8_t bad_labels[]={0,1};
    double result=-9,grad[]={-9,-9,-9,-9};
    assert(hinge_loss(scores,labels,4,&result,grad));
    assert(fabs(result-.875)<1e-12);
    assert(grad[0]==0 && grad[1]==-.25 && grad[2]==.25 && grad[3]==.25);
    assert(hinge_loss(boundary,boundary_labels,2,&result,grad));
    assert(result==0 && grad[0]==0 && grad[1]==0);
    result=-9;grad[0]=-9;
    assert(!hinge_loss(scores,labels,0,&result,grad));
    assert(!hinge_loss(invalid,boundary_labels,2,&result,grad));
    assert(!hinge_loss(boundary,bad_labels,2,&result,grad));
    assert(!hinge_loss(extremes,boundary_labels,2,&result,grad));
    assert(!hinge_loss(scores,labels,4,NULL,grad));
    assert(result==-9 && grad[0]==-9);
    puts("C hinge loss tests passed");
    return 0;
}
