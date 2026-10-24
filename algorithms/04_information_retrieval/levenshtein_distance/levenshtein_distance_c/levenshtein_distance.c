#include "levenshtein_distance.h"
#include <stdint.h>
#include <stdlib.h>

static bool checked_length(const char *value,size_t *len) {
    if (!value) return false;
    for (size_t i=0;i<=512;++i) {
        const unsigned char ch=(unsigned char)value[i];
        if (ch==0) {
            *len=i;
            return true;
        }
        if (ch<32 || ch>126) return false;
    }
    return false;
}

bool levenshtein_distance(const char *a,const char *b,size_t *distance_out) {
    if (!distance_out) return false;
    size_t n,m;
    if (!checked_length(a,&n) || !checked_length(b,&m)) return false;
    if (m>n) {
        const char *temp=a;a=b;b=temp;
        const size_t length=n;n=m;m=length;
    }
    size_t *prev=malloc((m+1)*sizeof(*prev));
    size_t *curr=malloc((m+1)*sizeof(*curr));
    if (!prev || !curr) {free(prev);free(curr);return false;}
    for (size_t j=0;j<=m;++j) prev[j]=j;
    for (size_t i=1;i<=n;++i) {
        curr[0]=i;
        for (size_t j=1;j<=m;++j) {
            const size_t deletion=prev[j]+1;
            const size_t insertion=curr[j-1]+1;
            const size_t substitute=prev[j-1]+(a[i-1]!=b[j-1]);
            const size_t smaller=deletion<insertion?deletion:insertion;
            curr[j]=smaller<substitute?smaller:substitute;
        }
        size_t *tmp=prev;prev=curr;curr=tmp;
    }
    *distance_out=prev[m];
    free(prev);free(curr);
    return true;
}
