#include "no_repeat_ngram.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const size_t t[]={1,2,3,1,2}, bigrams[]={1,2,1,2}, bad[]={1,9};
    bool blocked[5]={true,true,true,true,true};
    assert(no_repeat_ngram(t,5,5,3,blocked));
    assert(!blocked[0] && !blocked[1] && !blocked[2] && blocked[3] && !blocked[4]);
    assert(no_repeat_ngram(bigrams,4,5,2,blocked));
    assert(blocked[1] && !blocked[0] && !blocked[2]);
    assert(no_repeat_ngram(t,5,5,1,blocked));
    assert(!blocked[0] && blocked[1] && blocked[2] && blocked[3]);
    assert(no_repeat_ngram(t,2,5,3,blocked));
    for (size_t i=0;i<5;++i) assert(!blocked[i]);
    assert(no_repeat_ngram(NULL,0,5,1,blocked));
    for (size_t i=0;i<5;++i) assert(!blocked[i]);
    blocked[0]=true;
    assert(!no_repeat_ngram(NULL,1,5,1,blocked));
    assert(!no_repeat_ngram(bad,2,5,1,blocked));
    assert(!no_repeat_ngram(t,5,0,2,blocked));
    assert(!no_repeat_ngram(t,5,4097,2,blocked));
    assert(!no_repeat_ngram(t,5,5,0,blocked));
    assert(!no_repeat_ngram(t,5,5,33,blocked));
    assert(!no_repeat_ngram(t,16385,5,2,blocked));
    assert(!no_repeat_ngram(t,5,5,2,NULL));
    assert(blocked[0]);
    puts("C no-repeat n-gram tests passed");
    return 0;
}
