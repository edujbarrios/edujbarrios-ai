#include "hamming_radius_search.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const uint32_t values[]={3,1,7,15,2};
    const uint32_t duplicates[]={3,3,1};
    const uint32_t invalid[]={3,UINT32_MAX};
    const uint32_t full[]={0,(UINT32_C(1)<<30)-1};
    size_t out[]={99,99,99,99,99};
    size_t count=99;
    assert(hamming_radius_search(3,values,5,1,out,&count));
    assert(count==4 && out[0]==0 && out[1]==1 && out[2]==2 && out[3]==4);
    assert(hamming_radius_search(3,values,5,0,out,&count));
    assert(count==1 && out[0]==0);
    assert(hamming_radius_search(3,values,5,30,out,&count));
    assert(count==5 && out[0]==0 && out[1]==1 && out[2]==2 &&
           out[3]==4 && out[4]==3);
    assert(hamming_radius_search(3,duplicates,3,0,out,&count));
    assert(count==2 && out[0]==0 && out[1]==1);
    assert(hamming_radius_search(0,full,2,30,out,&count));
    assert(count==2 && out[0]==0 && out[1]==1);
    assert(hamming_radius_search(0,NULL,0,0,NULL,&count) && count==0);
    out[0]=99;count=99;
    assert(!hamming_radius_search(3,invalid,2,0,out,&count));
    assert(!hamming_radius_search(UINT32_MAX,values,5,0,out,&count));
    assert(!hamming_radius_search(3,values,5,31,out,&count));
    assert(!hamming_radius_search(3,values,5,1,NULL,&count));
    assert(!hamming_radius_search(3,values,5,1,out,NULL));
    assert(out[0]==99 && count==99);
    puts("C hamming radius tests passed");
    return 0;
}
