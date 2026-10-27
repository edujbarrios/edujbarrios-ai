#include "image_text_mrr.h"
#include <math.h>

bool image_text_mrr(const double *images, const double *texts,
                    size_t n, size_t dim, double result[2]) {
    if (!images || !texts || !result || n<2 || n>64 ||
        dim<1 || dim>128) return false;
    double visual[64*128], language[64*128], sim[64*64];
    for (size_t kind=0; kind<2; ++kind) {
        const double *src = kind ? texts : images;
        double *dst = kind ? language : visual;
        for (size_t i=0; i<n; ++i) {
            double norm2=0.0;
            for (size_t d=0; d<dim; ++d) {
                const double x=src[i*dim+d];
                if (!isfinite(x) || fabs(x)>1000.0) return false;
                norm2+=x*x;
            }
            if (norm2<=0) return false;
            const double norm=sqrt(norm2);
            for (size_t d=0; d<dim; ++d)
                dst[i*dim+d]=src[i*dim+d]/norm;
        }
    }
    for (size_t i=0; i<n; ++i) for (size_t j=0; j<n; ++j) {
        double dot=0.0;
        for (size_t d=0; d<dim; ++d)
            dot+=visual[i*dim+d]*language[j*dim+d];
        sim[i*n+j]=dot;
    }
    double row_total=0.0, col_total=0.0;
    for (size_t i=0; i<n; ++i) {
        size_t row_rank=1, col_rank=1;
        const double correct=sim[i*n+i];
        for (size_t j=0; j<n; ++j) {
            const double row=sim[i*n+j], col=sim[j*n+i];
            if (row>correct || (row==correct && j<i)) ++row_rank;
            if (col>correct || (col==correct && j<i)) ++col_rank;
        }
        row_total+=1.0/(double)row_rank;
        col_total+=1.0/(double)col_rank;
    }
    result[0]=row_total/(double)n;
    result[1]=col_total/(double)n;
    return true;
}
