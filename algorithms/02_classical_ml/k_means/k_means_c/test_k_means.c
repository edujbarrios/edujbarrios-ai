#include "k_means.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double points[] = {0,0,10,10,0,2,10,12};
    const double single[] = {0,0,10,10};
    const double duplicate[] = {0,0,1,1};
    const double bad[] = {NAN,1,0,1};
    const double overflow[] = {1e308,1e308};
    double centers[4] = {-9,-9,-9,-9};
    size_t labels[4] = {99,99,99,99};

    assert(k_means(points,4,2,2,10,centers,labels));
    assert(centers[0]==0 && centers[1]==1 &&
           centers[2]==10 && centers[3]==11);
    assert(labels[0]==0 && labels[1]==1 &&
           labels[2]==0 && labels[3]==1);
    assert(k_means(single,2,2,1,10,centers,labels));
    assert(centers[0]==5 && centers[1]==5);
    assert(labels[0]==0 && labels[1]==0);

    centers[0]=-9; labels[0]=99;
    assert(!k_means(points,4,2,0,10,centers,labels));
    assert(!k_means(points,4,2,5,10,centers,labels));
    assert(!k_means(points,4,2,2,0,centers,labels));
    assert(!k_means(duplicate,2,1,2,10,centers,labels));
    assert(!k_means(bad,2,2,2,10,centers,labels));
    assert(!k_means(overflow,2,1,1,10,centers,labels));
    assert(centers[0]==-9 && labels[0]==99);
    puts("C K-means tests passed");
    return 0;
}
