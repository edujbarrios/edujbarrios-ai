#include "macro_f1_score.h"

bool macro_f1_score(const uint8_t *actual, const uint8_t *predicted,
                    size_t n, size_t classes, double *score_out) {
    if (!actual || !predicted || !score_out || n<1 || n>512 ||
        classes<2 || classes>16) return false;
    size_t truth[16]={0}, guesses[16]={0}, hits[16]={0};
    for(size_t i=0;i<n;++i) {
        const size_t a=actual[i], p=predicted[i];
        if(a>=classes || p>=classes) return false;
        ++truth[a];
        ++guesses[p];
        if(a==p)++hits[a];
    }
    double sum=0.0;
    for(size_t c=0;c<classes;++c) {
        const size_t denominator=truth[c]+guesses[c];
        if(denominator>0) sum+=(2.0*(double)hits[c])/(double)denominator;
    }
    *score_out=sum/(double)classes;
    return true;
}
