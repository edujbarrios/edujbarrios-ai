#include "boyer_moore_horspool_search.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const char *text, const char *pattern,
                  const size_t *want, size_t expected_n) {
    size_t result[513] = {0}, n = 999;
    assert(boyer_moore_horspool_search(text,pattern,result,&n));
    assert(n == expected_n);
    for (size_t i=0;i<n;++i) assert(result[i] == want[i]);
}
int main(void) {
    const size_t one[] = {4}, overlap[] = {0,1,2};
    const size_t gaps[] = {0,2,4}, multiples[] = {0,3,6};
    const size_t mississippi[] = {1,4}, boundaries[] = {0,1,2,3};
    const size_t zero[] = {0};
    check("ABAAABCD","ABC",one,1);
    check("aaaa","aa",overlap,3);
    check("abababa","aba",gaps,3);
    check("ABCABCABC","ABC",multiples,3);
    check("mississippi","issi",mississippi,2);
    check("abc","XYZ",NULL,0);
    check("abc","ABC",NULL,0);
    check("ab","abc",NULL,0);
    check("abc","",boundaries,4);
    check("","",zero,1);
    check("","a",NULL,0);
    check("a","a",zero,1);
    char big[514];
    memset(big,'a',512);big[512]='\0';
    size_t all[513];
    for(size_t i=0;i<511;++i) all[i]=i;
    check(big,"aa",all,511);
    check(big,big,zero,1);
    size_t every[513];
    for(size_t i=0;i<513;++i) every[i]=i;
    check(big,"",every,513);
    big[512]='a';big[513]='\0';
    size_t p[513]={777}, count=777;
    assert(!boyer_moore_horspool_search(big,"a",p,&count));
    assert(!boyer_moore_horspool_search("a\tb","a",p,&count));
    assert(!boyer_moore_horspool_search("é","e",p,&count));
    assert(!boyer_moore_horspool_search(NULL,"a",p,&count));
    assert(!boyer_moore_horspool_search("a","a",NULL,&count));
    assert(!boyer_moore_horspool_search("a","a",p,NULL));
    assert(p[0]==777 && count==777);
    puts("C Horspool search tests passed");
    return 0;
}
