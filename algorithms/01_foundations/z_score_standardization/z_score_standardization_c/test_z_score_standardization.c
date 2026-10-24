#include "z_score_standardization.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double data[]={1,10,2,10,3,10};
    const double single[]={5,2};
    const double bad[]={1,NAN};
    const double huge[]={-1e308,1e308};
    double means[]={-9,-9},stds[]={-9,-9},out[]={-9,-9,-9,-9,-9,-9};
    assert(z_score_standardization(data,3,2,means,stds,out));
    assert(fabs(means[0]-2)<1e-12 && means[1]==10);
    assert(fabs(stds[0]-sqrt(2.0/3.0))<1e-12 && stds[1]==0);
    assert(fabs(out[0]+sqrt(1.5))<1e-12 && out[1]==0);
    assert(out[2]==0 && out[3]==0 && fabs(out[4]-sqrt(1.5))<1e-12);
    assert(z_score_standardization(single,1,2,means,stds,out));
    assert(means[0]==5 && means[1]==2);
    assert(stds[0]==0 && stds[1]==0 && out[0]==0 && out[1]==0);
    means[0]=-9;stds[0]=-9;out[0]=-9;
    assert(!z_score_standardization(bad,1,2,means,stds,out));
    assert(!z_score_standardization(huge,2,1,means,stds,out));
    assert(!z_score_standardization(data,0,2,means,stds,out));
    assert(!z_score_standardization(data,3,0,means,stds,out));
    assert(!z_score_standardization(data,3,2,NULL,stds,out));
    assert(means[0]==-9 && stds[0]==-9 && out[0]==-9);
    puts("C z-score standardization tests passed");
    return 0;
}
