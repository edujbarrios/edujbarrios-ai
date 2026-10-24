#include "greedy_k_medoids.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double points[]={0,1,9,10};
    const double repeated[]={0,0,0};
    const double bad[]={NAN,1};
    const double huge[]={-1.7e308,1.7e308};
    size_t medoids[]={99,99,99,99};
    assert(greedy_k_medoids(points,4,1,1,medoids) && medoids[0]==1);
    assert(greedy_k_medoids(points,4,1,2,medoids));
    assert(medoids[0]==1 && medoids[1]==2);
    assert(greedy_k_medoids(points,4,1,3,medoids));
    assert(medoids[0]==1 && medoids[1]==2 && medoids[2]==0);
    assert(greedy_k_medoids(repeated,3,1,3,medoids));
    assert(medoids[0]==0 && medoids[1]==1 && medoids[2]==2);
    assert(greedy_k_medoids(points,4,1,4,medoids));
    assert(medoids[0]==1 && medoids[1]==2 && medoids[2]==0 && medoids[3]==3);
    medoids[0]=99;
    assert(!greedy_k_medoids(points,4,1,0,medoids));
    assert(!greedy_k_medoids(points,4,1,5,medoids));
    assert(!greedy_k_medoids(bad,2,1,1,medoids));
    assert(!greedy_k_medoids(huge,2,1,1,medoids));
    assert(!greedy_k_medoids(points,0,1,1,medoids));
    assert(!greedy_k_medoids(points,4,1,1,NULL));
    assert(medoids[0]==99);
    puts("C greedy K-medoids tests passed");
    return 0;
}
