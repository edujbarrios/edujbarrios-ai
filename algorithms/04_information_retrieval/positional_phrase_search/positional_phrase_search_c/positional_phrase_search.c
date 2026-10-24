#include "positional_phrase_search.h"
#include <stdlib.h>

#define MAX_VALUE UINT32_C(1000000000)
static bool valid(const uint32_t *docs,const uint32_t *positions,size_t n){
    if (n>4096 || (n>0 && (!docs || !positions))) return false;
    for (size_t i=0;i<n;++i) {
        if (docs[i]>MAX_VALUE || positions[i]>MAX_VALUE) return false;
        if (i>0 && (docs[i]<docs[i-1] ||
            (docs[i]==docs[i-1] && positions[i]<=positions[i-1])))
            return false;
    }
    return true;
}
bool positional_phrase_search(
    const uint32_t *left_docs,const uint32_t *left_positions,size_t left_n,
    const uint32_t *right_docs,const uint32_t *right_positions,size_t right_n,
    uint32_t *result_docs,size_t *result_count){
    if (!result_docs || !result_count ||
        !valid(left_docs,left_positions,left_n) ||
        !valid(right_docs,right_positions,right_n)) return false;
    size_t capacity=left_n<right_n?left_n:right_n;
    uint32_t *scratch=malloc((capacity?capacity:1)*sizeof(*scratch));
    if (!scratch) return false;
    size_t a=0,b=0,count=0;
    while (a<left_n && b<right_n) {
        const uint32_t doc_a=left_docs[a],doc_b=right_docs[b];
        if (doc_a<doc_b) {
            do {++a;} while (a<left_n && left_docs[a]==doc_a);
        } else if (doc_b<doc_a) {
            do {++b;} while (b<right_n && right_docs[b]==doc_b);
        } else {
            size_t end_a=a,end_b=b;
            while (end_a<left_n && left_docs[end_a]==doc_a) ++end_a;
            while (end_b<right_n && right_docs[end_b]==doc_b) ++end_b;
            size_t pa=a,pb=b;
            bool found=false;
            while (pa<end_a && pb<end_b) {
                const uint32_t wanted=left_positions[pa]+1;
                const uint32_t observed=right_positions[pb];
                if (wanted==observed) {found=true;break;}
                if (wanted<observed) ++pa; else ++pb;
            }
            if (found) scratch[count++]=doc_a;
            a=end_a;b=end_b;
        }
    }
    for (size_t i=0;i<count;++i) result_docs[i]=scratch[i];
    *result_count=count;
    free(scratch);
    return true;
}
