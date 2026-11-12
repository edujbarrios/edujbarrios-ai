#include "single_link_clustering.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void equal(const size_t *actual, const size_t *expected, size_t n) {
    for (size_t i = 0; i < n; ++i) assert(actual[i] == expected[i]);
}
int main(void) {
    const int32_t points[] = {0,0, 0,1, 10,10, 10,11};
    const size_t expected2[] = {0,0,1,1}, expected3[] = {0,0,1,2};
    const size_t expected1[] = {0,0,0,0}, expected4[] = {0,1,2,3};
    const int32_t square[] = {0,0, 0,2, 2,0, 2,2};
    const size_t tied[] = {0,0,0,1};
    const int32_t duplicate[] = {0,0, 0,0, 10,10};
    const size_t dup_expected[] = {0,0,1};
    const int32_t edge[] = {-1000,-1000, 1000,1000};
    const int32_t invalid[] = {1001,0, 0,0};
    size_t result[32] = {0};
    assert(single_link_clustering(points,4,2,result)); equal(result,expected2,4);
    assert(single_link_clustering(points,4,3,result)); equal(result,expected3,4);
    assert(single_link_clustering(points,4,1,result)); equal(result,expected1,4);
    assert(single_link_clustering(points,4,4,result)); equal(result,expected4,4);
    assert(single_link_clustering(square,4,2,result)); equal(result,tied,4);
    assert(single_link_clustering(duplicate,3,2,result)); equal(result,dup_expected,3);
    assert(single_link_clustering(edge,2,1,result)); assert(result[0]==0 && result[1]==0);
    assert(single_link_clustering(points,1,1,result)); assert(result[0] == 0);
    int32_t many[64] = {0};
    for (size_t i = 0; i < 32; ++i) many[2*i] = (int32_t)i;
    assert(single_link_clustering(many,32,1,result));
    for (size_t i = 0; i < 32; ++i) assert(result[i] == 0);
    assert(single_link_clustering(many,32,32,result));
    for (size_t i = 0; i < 32; ++i) assert(result[i] == i);
    result[0] = 99;
    assert(!single_link_clustering(NULL,4,2,result));
    assert(!single_link_clustering(points,0,1,result));
    assert(!single_link_clustering(points,33,2,result));
    assert(!single_link_clustering(points,4,0,result));
    assert(!single_link_clustering(points,4,5,result));
    assert(!single_link_clustering(invalid,2,1,result));
    assert(!single_link_clustering(points,4,2,NULL));
    assert(result[0] == 99);
    puts("C single-link clustering tests passed");
    return 0;
}
