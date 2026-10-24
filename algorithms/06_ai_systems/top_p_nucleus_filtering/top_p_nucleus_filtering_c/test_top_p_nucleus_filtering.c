#include "top_p_nucleus_filtering.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double p[]={.4,.3,.2,.1};
    const double tied[]={.5,.5,0},single[]={1};
    const double invalid[]={.7,.7},negative[]={1.1,-.1};
    const double nan[]={NAN,1};
    size_t ids[]={99,99,99,99},count=99;
    assert(top_p_nucleus_filtering(p,4,.6,ids,&count));
    assert(count==2 && ids[0]==0 && ids[1]==1);
    assert(top_p_nucleus_filtering(p,4,.85,ids,&count));
    assert(count==3 && ids[0]==0 && ids[1]==1 && ids[2]==2);
    assert(top_p_nucleus_filtering(p,4,.95,ids,&count));
    assert(count==4 && ids[3]==3);
    assert(top_p_nucleus_filtering(tied,3,.5,ids,&count));
    assert(count==1 && ids[0]==0);
    assert(top_p_nucleus_filtering(tied,3,.75,ids,&count));
    assert(count==2 && ids[0]==0 && ids[1]==1);
    assert(top_p_nucleus_filtering(tied,3,1,ids,&count));
    assert(count==2 && ids[0]==0 && ids[1]==1);
    assert(top_p_nucleus_filtering(single,1,1,ids,&count));
    assert(count==1 && ids[0]==0);
    ids[0]=99;count=99;
    assert(!top_p_nucleus_filtering(invalid,2,.5,ids,&count));
    assert(!top_p_nucleus_filtering(negative,2,.5,ids,&count));
    assert(!top_p_nucleus_filtering(nan,2,.5,ids,&count));
    assert(!top_p_nucleus_filtering(p,4,0,ids,&count));
    assert(!top_p_nucleus_filtering(p,4,1.1,ids,&count));
    assert(!top_p_nucleus_filtering(p,0,.5,ids,&count));
    assert(!top_p_nucleus_filtering(p,4,.5,NULL,&count));
    assert(!top_p_nucleus_filtering(p,4,.5,ids,NULL));
    assert(ids[0]==99 && count==99);
    puts("C nucleus top-p tests passed");
    return 0;
}
