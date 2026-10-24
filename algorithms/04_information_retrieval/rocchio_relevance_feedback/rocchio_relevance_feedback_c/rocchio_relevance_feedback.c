#include "rocchio_relevance_feedback.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool rocchio_relevance_feedback(const double *query,size_t d,
    const double *relevant,size_t nr,const double *nonrelevant,size_t ni,
    double *output){
    if (!query || !output || d==0 || d>64 ||
        nr>128 || ni>128 || nr+ni==0 ||
        (nr>0 && !relevant) || (ni>0 && !nonrelevant))
        return false;
    for (size_t j=0;j<d;++j) {
        if (!isfinite(query[j]) || query[j]<0 || query[j]>1e6)
            return false;
    }
    for (size_t i=0;i<nr*d;++i) {
        if (!isfinite(relevant[i]) || relevant[i]<0 || relevant[i]>1e6)
            return false;
    }
    for (size_t i=0;i<ni*d;++i) {
        if (!isfinite(nonrelevant[i]) ||
            nonrelevant[i]<0 || nonrelevant[i]>1e6)
            return false;
    }
    double *scratch=malloc(d*sizeof(*scratch));
    if (!scratch) return false;
    for (size_t j=0;j<d;++j) {
        double pos=0.0,neg=0.0;
        for (size_t i=0;i<nr;++i) pos+=relevant[i*d+j]/(double)nr;
        for (size_t i=0;i<ni;++i) neg+=nonrelevant[i*d+j]/(double)ni;
        const double updated=query[j]+0.75*pos-0.15*neg;
        if (!isfinite(pos) || !isfinite(neg) || !isfinite(updated)) {
            free(scratch);return false;
        }
        scratch[j]=updated>0.0?updated:0.0;
    }
    memcpy(output,scratch,d*sizeof(*scratch));
    free(scratch);
    return true;
}
