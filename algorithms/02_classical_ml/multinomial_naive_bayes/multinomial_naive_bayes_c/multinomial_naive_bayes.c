#include "multinomial_naive_bayes.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COUNT UINT32_C(1000000)
#define MAX_EXACT_SUM 9007199254740991.0

bool multinomial_naive_bayes(const uint32_t *counts,const uint8_t *labels,
    size_t n,size_t d,double *log_priors,double *log_likelihoods) {
    if (!counts || !labels || !log_priors || !log_likelihoods ||
        n==0 || d==0 || n>SIZE_MAX/d ||
        d>SIZE_MAX/(2*sizeof(double))) return false;
    double totals[2]={0.0,0.0}, members[2]={0.0,0.0};
    double *tokens=calloc(2*d,sizeof(*tokens));
    double *likelihoods=malloc(2*d*sizeof(*likelihoods));
    if (!tokens || !likelihoods) {free(tokens);free(likelihoods);return false;}
    for (size_t i=0;i<n;++i) {
        if (labels[i]>1) goto fail;
        const unsigned c=labels[i];
        members[c]+=1.0;
        if (!isfinite(members[c])) goto fail;
        for (size_t j=0;j<d;++j) {
            const uint32_t count=counts[i*d+j];
            if (count>MAX_COUNT) goto fail;
            tokens[c*d+j]+=(double)count;
            totals[c]+=(double)count;
            if (tokens[c*d+j]>MAX_EXACT_SUM || totals[c]>MAX_EXACT_SUM)
                goto fail;
        }
    }
    if (members[0]==0.0 || members[1]==0.0) goto fail;
    double priors[2]={log(members[0]/(double)n),log(members[1]/(double)n)};
    if (!isfinite(priors[0]) || !isfinite(priors[1])) goto fail;
    for (size_t c=0;c<2;++c) {
        const double denominator=totals[c]+(double)d;
        if (!isfinite(denominator) || denominator<=0.0) goto fail;
        for (size_t j=0;j<d;++j) {
            likelihoods[c*d+j]=log((tokens[c*d+j]+1.0)/denominator);
            if (!isfinite(likelihoods[c*d+j])) goto fail;
        }
    }
    memcpy(log_priors,priors,sizeof(priors));
    memcpy(log_likelihoods,likelihoods,2*d*sizeof(*likelihoods));
    free(tokens);free(likelihoods);
    return true;
fail:
    free(tokens);free(likelihoods);
    return false;
}
bool multinomial_naive_bayes_predict(const double *log_priors,
    const double *log_likelihoods,const uint32_t *query,
    size_t d,uint8_t *prediction) {
    if (!log_priors || !log_likelihoods || !query || !prediction ||
        d==0 || d>SIZE_MAX/2) return false;
    if (!isfinite(log_priors[0]) || !isfinite(log_priors[1])) return false;
    for (size_t j=0;j<2*d;++j) if (!isfinite(log_likelihoods[j])) return false;
    double scores[2]={log_priors[0],log_priors[1]};
    for (size_t j=0;j<d;++j) {
        if (query[j]>MAX_COUNT) return false;
        for (size_t c=0;c<2;++c) {
            const double term=(double)query[j]*log_likelihoods[c*d+j];
            if (!isfinite(term)) return false;
            scores[c]+=term;
            if (!isfinite(scores[c])) return false;
        }
    }
    *prediction=(uint8_t)(scores[1]>scores[0]);
    return true;
}
