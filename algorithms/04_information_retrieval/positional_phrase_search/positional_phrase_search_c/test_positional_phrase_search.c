#include "positional_phrase_search.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const uint32_t ld[]={1,1,2,4},lp[]={0,5,1,2};
    const uint32_t rd[]={1,1,2,4},rp[]={1,8,3,3};
    const uint32_t unsorted[]={1,1,2,4},unsorted_pos[]={5,0,1,2};
    const uint32_t repeated_docs[]={1,1},repeated_pos[]={0,0};
    const uint32_t invalid_docs[]={1000000001},single_pos[]={0};
    const uint32_t nonadjacent_left[]={5},nonadjacent_right[]={5};
    const uint32_t position_two[]={2},position_one[]={1};
    uint32_t results[]={99,99,99,99};
    size_t count=99;
    assert(positional_phrase_search(ld,lp,4,rd,rp,4,results,&count));
    assert(count==2 && results[0]==1 && results[1]==4);
    assert(positional_phrase_search(NULL,NULL,0,rd,rp,4,results,&count));
    assert(count==0);
    assert(positional_phrase_search(ld,lp,4,NULL,NULL,0,results,&count));
    assert(count==0);
    assert(positional_phrase_search(nonadjacent_left,position_two,1,
                                    nonadjacent_right,position_one,1,results,&count));
    assert(count==0);
    count=99;results[0]=99;
    assert(!positional_phrase_search(unsorted,unsorted_pos,4,rd,rp,4,results,&count));
    assert(!positional_phrase_search(repeated_docs,repeated_pos,2,rd,rp,4,results,&count));
    assert(!positional_phrase_search(invalid_docs,single_pos,1,rd,rp,4,results,&count));
    assert(!positional_phrase_search(ld,lp,4,rd,rp,4,NULL,&count));
    assert(!positional_phrase_search(ld,lp,4,rd,rp,4,results,NULL));
    assert(count==99 && results[0]==99);
    puts("C positional phrase search tests passed");
    return 0;
}
