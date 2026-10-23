#include "random_hyperplane_hash.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {1,2};
    const double normals[] = {1,0, 0,1, -1,-1};
    const double tie[] = {0,1};
    const double zero[] = {0,0};
    const double huge[] = {1e308};
    const double bad[] = {NAN,1};
    const double scalar[] = {1};
    double thirty[30];
    for (size_t j=0;j<30;++j) thirty[j]=1;
    uint32_t signature = 99;
    assert(random_hyperplane_hash(x,2,normals,3,&signature));
    assert(signature == 3);
    assert(random_hyperplane_hash(tie,2,normals,3,&signature));
    assert(signature == 3);
    assert(random_hyperplane_hash(scalar,1,thirty,30,&signature));
    assert(signature == ((1u<<30)-1u));
    signature = 99;
    assert(!random_hyperplane_hash(zero,2,normals,3,&signature));
    assert(!random_hyperplane_hash(x,2,zero,1,&signature));
    assert(!random_hyperplane_hash(bad,2,normals,3,&signature));
    assert(!random_hyperplane_hash(huge,1,huge,1,&signature));
    assert(!random_hyperplane_hash(x,2,normals,31,&signature));
    assert(!random_hyperplane_hash(x,0,normals,3,&signature));
    assert(signature == 99);
    puts("C random hyperplane hash tests passed");
    return 0;
}
