#include "jaro_winkler_similarity.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static void check(const char *a, const char *b, double expected) {
    double actual = -1.0;
    assert(jaro_winkler_similarity(a, b, &actual));
    assert(fabs(actual - expected) < 1e-12);
}

int main(void) {
    check("MARTHA", "MARHTA", 0.9611111111111111);
    check("DIXON", "DICKSONX", 0.8133333333333334);
    check("JELLYFISH", "SMELLYFISH", 0.8962962962962963);
    check("ABCD", "ACBD", 0.925);
    check("abc", "axz", 5.0 / 9.0);
    check("MARHTA", "MARTHA", 0.9611111111111111);
    check("", "", 1.0);
    check("", "abc", 0.0);
    check("abc", "", 0.0);
    check("x", "x", 1.0);
    check("x", "y", 0.0);
    check("A", "a", 0.0);
    char long_text[514];
    memset(long_text, 'a', 512);
    long_text[512] = '\0';
    check(long_text, long_text, 1.0);
    long_text[512] = 'a';
    long_text[513] = '\0';
    double result = 42.0;
    assert(!jaro_winkler_similarity(long_text, "x", &result));
    assert(!jaro_winkler_similarity("x", "b\tc", &result));
    assert(!jaro_winkler_similarity("é", "e", &result));
    assert(!jaro_winkler_similarity(NULL, "e", &result));
    assert(!jaro_winkler_similarity("e", "e", NULL));
    assert(result == 42.0);
    puts("C Jaro-Winkler tests passed");
    return 0;
}
