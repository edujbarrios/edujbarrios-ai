#include "levenshtein_distance.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void) {
    size_t result=99;
    assert(levenshtein_distance("kitten","sitting",&result) && result==3);
    assert(levenshtein_distance("sitting","kitten",&result) && result==3);
    assert(levenshtein_distance("","abc",&result) && result==3);
    assert(levenshtein_distance("abc","",&result) && result==3);
    assert(levenshtein_distance("cat","cat",&result) && result==0);
    assert(levenshtein_distance("ab","ba",&result) && result==2);
    assert(levenshtein_distance("A","a",&result) && result==1);
    char big[514];
    memset(big,'a',512);big[512]='\0';
    assert(levenshtein_distance(big,"",&result) && result==512);
    big[512]='a';big[513]='\0';
    result=99;
    assert(!levenshtein_distance(big,"",&result));
    assert(!levenshtein_distance("a\tb","ab",&result));
    assert(!levenshtein_distance("\x01","a",&result));
    assert(!levenshtein_distance("é","e",&result));
    assert(!levenshtein_distance(NULL,"b",&result));
    assert(!levenshtein_distance("a","b",NULL));
    assert(result==99);
    puts("C Levenshtein distance tests passed");
    return 0;
}
