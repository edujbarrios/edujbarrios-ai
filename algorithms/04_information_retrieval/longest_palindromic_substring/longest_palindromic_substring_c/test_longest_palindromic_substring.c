#include "longest_palindromic_substring.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const char *s, size_t length, size_t start) {
    size_t got_length = 999, got_start = 999;
    assert(longest_palindromic_substring(s, &got_length, &got_start));
    assert(got_length == length && got_start == start);
}
int main(void) {
    check("babad",3,0);
    check("cbbd",2,1);
    check("aaaa",4,0);
    check("abc",1,0);
    check("racecar",7,0);
    check("abacdfgdcaba",3,0);
    check("a b a",5,0);
    check("a!a",3,0);
    check("abbaxabba",9,0);
    check("",0,0);
    check("z",1,0);
    char large[514];
    memset(large,'a',512);
    large[512]='\0';
    check(large,512,0);
    large[512]='a'; large[513]='\0';
    size_t l = 777, s = 777;
    assert(!longest_palindromic_substring(large,&l,&s));
    assert(!longest_palindromic_substring("é",&l,&s));
    assert(!longest_palindromic_substring("a\tb",&l,&s));
    assert(!longest_palindromic_substring(NULL,&l,&s));
    assert(!longest_palindromic_substring("a",NULL,&s));
    assert(!longest_palindromic_substring("a",&l,NULL));
    assert(!longest_palindromic_substring("a",&l,&l));
    assert(l==777 && s==777);
    puts("C longest palindrome tests passed");
    return 0;
}
