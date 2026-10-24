#include "markov_beam_search.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
struct hypothesis {double score;uint8_t tokens[8];};
static int compare(const void *a,const void *b) {
    const struct hypothesis *x=a,*y=b;
    if (x->score>y->score) return -1;
    if (x->score<y->score) return 1;
    for (size_t i=0;i<8;++i) {
        if (x->tokens[i]<y->tokens[i]) return -1;
        if (x->tokens[i]>y->tokens[i]) return 1;
    }
    return 0;
}
static bool valid_row(const double *values,size_t count) {
    double sum=0;
    for (size_t i=0;i<count;++i) {
        const double p=values[i];
        if (!isfinite(p) || p<=0 || p>1) return false;
        sum+=p;
    }
    return isfinite(sum) && fabs(sum-1.0)<=1e-9;
}
bool markov_beam_search(const double *initial,const double *transition,
    size_t vocabulary,size_t length,size_t width,
    uint8_t *tokens,double *log_probability) {
    if (!initial || !transition || !tokens || !log_probability ||
        vocabulary==0 || vocabulary>8 || length==0 || length>8 ||
        width==0 || width>8 || !valid_row(initial,vocabulary)) return false;
    for (size_t i=0;i<vocabulary;++i)
        if (!valid_row(transition+i*vocabulary,vocabulary)) return false;
    struct hypothesis beam[8]={{0}};
    struct hypothesis candidates[64];
    size_t active=1;
    for (size_t step=0;step<length;++step) {
        size_t count=0;
        for (size_t i=0;i<active;++i) {
            for (size_t token=0;token<vocabulary;++token) {
                const double p=step==0?initial[token]:
                    transition[(size_t)beam[i].tokens[step-1]*vocabulary+token];
                struct hypothesis next=beam[i];
                next.tokens[step]=(uint8_t)token;
                next.score+=log(p);
                if (!isfinite(next.score)) return false;
                candidates[count++]=next;
            }
        }
        qsort(candidates,count,sizeof(*candidates),compare);
        active=count<width?count:width;
        memcpy(beam,candidates,active*sizeof(*beam));
    }
    memcpy(tokens,beam[0].tokens,length*sizeof(*tokens));
    *log_probability=beam[0].score;
    return true;
}
