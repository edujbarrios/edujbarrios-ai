#include "nearest_centroid_classifier.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={0,0,2,0,8,0,10,0};
    const uint8_t labels[]={0,0,1,1};
    const uint8_t missing[]={0,0,0,0};
    const uint8_t bad_labels[]={0,0,1,2};
    const double left[]={2,0},right[]={8,0},tie[]={5,0};
    const double bad_x[]={NAN,0,2,0,8,0,10,0};
    const double overflow_x[]={1e308,1e308,0};
    const uint8_t overflow_labels[]={0,0,1};
    double means[]={-9,-9,-9,-9};
    uint8_t predicted=9;
    assert(nearest_centroid_classifier(x,labels,4,2,means));
    assert(means[0]==1 && means[1]==0 && means[2]==9 && means[3]==0);
    assert(nearest_centroid_classifier_predict(means,left,2,&predicted)&&predicted==0);
    assert(nearest_centroid_classifier_predict(means,right,2,&predicted)&&predicted==1);
    assert(nearest_centroid_classifier_predict(means,tie,2,&predicted)&&predicted==0);
    means[0]=-9;predicted=9;
    assert(!nearest_centroid_classifier(x,missing,4,2,means));
    assert(!nearest_centroid_classifier(x,bad_labels,4,2,means));
    assert(!nearest_centroid_classifier(bad_x,labels,4,2,means));
    assert(!nearest_centroid_classifier(overflow_x,overflow_labels,3,1,means));
    assert(!nearest_centroid_classifier(x,labels,1,2,means));
    assert(means[0]==-9);
    const double invalid_centers[]={NAN,0,9,0};
    assert(!nearest_centroid_classifier_predict(invalid_centers,right,2,&predicted));
    assert(predicted==9);
    puts("C nearest centroid tests passed");
    return 0;
}
