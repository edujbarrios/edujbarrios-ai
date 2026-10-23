#include "reciprocal_rank_fusion.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const int a[] = {10,20,30};
    const int b[] = {30,10,40};
    const int tie_a[] = {2,1};
    const int tie_b[] = {1,2};
    const int duplicate[] = {4,4};
    const int invalid[] = {-1};
    int result[8] = {-7};
    size_t count = 777;
    assert(reciprocal_rank_fusion(a,3,b,3,result,&count));
    assert(count==4 && result[0]==10 && result[1]==30 &&
           result[2]==20 && result[3]==40);
    assert(reciprocal_rank_fusion(tie_a,2,tie_b,2,result,&count));
    assert(count==2 && result[0]==1 && result[1]==2);
    assert(reciprocal_rank_fusion(a,3,NULL,0,result,&count));
    assert(count==3 && result[0]==10 && result[1]==20 && result[2]==30);
    result[0]=-7; count=777;
    assert(!reciprocal_rank_fusion(NULL,0,NULL,0,result,&count));
    assert(!reciprocal_rank_fusion(duplicate,2,b,3,result,&count));
    assert(!reciprocal_rank_fusion(invalid,1,b,3,result,&count));
    assert(!reciprocal_rank_fusion(a,3,b,3,NULL,&count));
    assert(result[0]==-7 && count==777);
    puts("C reciprocal rank fusion tests passed");
    return 0;
}
