#include "label_smoothed_cross_entropy.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double z[]={0,1.0986122886681098};
    const double same[]={0,0};
    const double singleton[]={12};
    const double shifted[]={1000,1001};
    const double extreme[]={-1e308,1e308};
    const double invalid[]={NAN,0};
    double loss=-9,g[]={-9,-9};
    assert(label_smoothed_cross_entropy(z,2,1,.2,&loss,g));
    assert(fabs(loss-(-.1*log(.25)-.9*log(.75)))<1e-12);
    assert(fabs(g[0]-.15)<1e-12 && fabs(g[1]+.15)<1e-12);
    assert(label_smoothed_cross_entropy(z,2,1,0,&loss,g));
    assert(fabs(loss+log(.75))<1e-12);
    assert(fabs(g[0]-.25)<1e-12 && fabs(g[1]+.25)<1e-12);
    assert(label_smoothed_cross_entropy(z,2,1,1,&loss,g));
    assert(fabs(g[0]+.25)<1e-12 && fabs(g[1]-.25)<1e-12);
    assert(label_smoothed_cross_entropy(same,2,0,.5,&loss,g));
    assert(fabs(loss-log(2))<1e-12);
    assert(label_smoothed_cross_entropy(singleton,1,0,.6,&loss,g));
    assert(loss==0 && g[0]==0);
    assert(label_smoothed_cross_entropy(shifted,2,1,.2,&loss,g));
    assert(isfinite(loss));
    loss=-9;g[0]=-9;
    assert(!label_smoothed_cross_entropy(z,0,0,.2,&loss,g));
    assert(!label_smoothed_cross_entropy(z,2,2,.2,&loss,g));
    assert(!label_smoothed_cross_entropy(z,2,1,-.1,&loss,g));
    assert(!label_smoothed_cross_entropy(z,2,1,1.1,&loss,g));
    assert(!label_smoothed_cross_entropy(invalid,2,1,.2,&loss,g));
    assert(!label_smoothed_cross_entropy(extreme,2,1,.2,&loss,g));
    assert(!label_smoothed_cross_entropy(z,2,1,.2,NULL,g));
    assert(loss==-9 && g[0]==-9);
    puts("C label-smoothed cross-entropy tests passed");
    return 0;
}
