#include "z_algorithm_substring_search.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const char *text, const char *pattern,
                  const size_t *want, size_t expected_n) {
    size_t result[513]={0}, n=999;
    assert(z_algorithm_substring_search(text,pattern,result,&n));
    assert(n==expected_n);
    for(size_t i=0;i<n;++i)assert(result[i]==want[i]);
}
int main(void) {
    const size_t banana[]={1,3}, overlap[]={0,1,2};
    const size_t pairs[]={0,3}, miss[]={1,4}, spaces[]={0,2};
    const size_t zero[]={0}, boundaries[]={0,1,2,3};
    check("banana","ana",banana,2);
    check("aaaa","aa",overlap,3);
    check("abcabc","abc",pairs,2);
    check("abc","xyz",NULL,0);
    check("abc","ABC",NULL,0);
    check("ab","abc",NULL,0);
    check("mississippi","issi",miss,2);
    check("a a a","a ",spaces,2);
    check("~!~!","~!",spaces,2);
    check("aba","aba",zero,1);
    check("abc","",boundaries,4);
    check("","",zero,1);
    check("","a",NULL,0);
    char long_text[514];
    memset(long_text,'a',512);long_text[512]='\0';
    size_t every[513];
    for(size_t i=0;i<513;++i)every[i]=i;
    check(long_text,"aa",every,511);
    check(long_text,long_text,zero,1);
    check(long_text,"",every,513);
    long_text[512]='a';long_text[513]='\0';
    size_t positions[513]={777},count=777;
    assert(!z_algorithm_substring_search(long_text,"a",positions,&count));
    assert(!z_algorithm_substring_search("a\tb","a",positions,&count));
    assert(!z_algorithm_substring_search("é","e",positions,&count));
    assert(!z_algorithm_substring_search(NULL,"a",positions,&count));
    assert(!z_algorithm_substring_search("a","a",NULL,&count));
    assert(!z_algorithm_substring_search("a","a",positions,NULL));
    assert(!z_algorithm_substring_search("a","a",positions,positions));
    assert(positions[0]==777 && count==777);
    puts("C Z-algorithm search tests passed");
    return 0;
}
