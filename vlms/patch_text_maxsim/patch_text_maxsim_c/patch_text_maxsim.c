#include "patch_text_maxsim.h"
#include <math.h>

bool patch_text_maxsim(const double *patches, size_t n_patches,
                       const double *tokens, size_t n_tokens, size_t dim,
                       size_t *matches, double *score) {
    if (!patches || !tokens || !matches || !score ||
        n_patches<1 || n_patches>32 || n_tokens<1 || n_tokens>32 ||
        dim<1 || dim>64) return false;
    double visual[32*64], language[32*64];
    for (size_t which=0; which<2; ++which) {
        const double *src=which ? tokens : patches;
        double *dst=which ? language : visual;
        const size_t count=which ? n_tokens : n_patches;
        for(size_t r=0;r<count;++r) {
            double norm2=0.0;
            for(size_t d=0;d<dim;++d) {
                const double v=src[r*dim+d];
                if(!isfinite(v) || fabs(v)>1000.0) return false;
                norm2+=v*v;
            }
            if(norm2<=0) return false;
            const double norm=sqrt(norm2);
            for(size_t d=0;d<dim;++d)
                dst[r*dim+d]=src[r*dim+d]/norm;
        }
    }
    size_t selected[32];
    double sum=0.0;
    for(size_t t=0;t<n_tokens;++t) {
        size_t best_id=0;
        double best=-INFINITY;
        for(size_t p=0;p<n_patches;++p) {
            double dot=0.0;
            for(size_t d=0;d<dim;++d)
                dot+=language[t*dim+d]*visual[p*dim+d];
            if(dot>best) {best=dot;best_id=p;}
        }
        selected[t]=best_id;
        sum+=best;
    }
    for(size_t t=0;t<n_tokens;++t) matches[t]=selected[t];
    *score=sum/(double)n_tokens;
    return true;
}
