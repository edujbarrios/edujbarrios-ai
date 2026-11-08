#include "character_bigram_jaccard.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static void check(const char *a, const char *b, double expected) {
    double result = -1;
    assert(character_bigram_jaccard(a, b, &result));
    assert(fabs(result - expected) < 1e-12);
}

int main(void) {
    check("banana","bandana",0.6);
    check("bandana","banana",0.6);
    check("aaaa","aa",1.0);
    check("abcd","cdef",0.2);
    check("abc","xyz",0.0);
    check("abc","abcd",2.0/3.0);
    check("AB","ab",0.0);
    check("","",1.0);
    check("a","",1.0);
    check("a","b",1.0);
    check("a","ab",0.0);
    check("~!","~!",1.0);
    char max[514];
    memset(max,'a',512);
    max[512]='\0';
    check(max,"aa",1.0);
    max[512]='a';
    max[513]='\0';
    double result=777;
    assert(!character_bigram_jaccard(max,"a",&result));
    assert(!character_bigram_jaccard("é","e",&result));
    assert(!character_bigram_jaccard("a\tb","a",&result));
    assert(!character_bigram_jaccard(NULL,"a",&result));
    assert(!character_bigram_jaccard("a","a",NULL));
    assert(result==777);
    puts("C character bigram Jaccard tests passed");
    return 0;
}
