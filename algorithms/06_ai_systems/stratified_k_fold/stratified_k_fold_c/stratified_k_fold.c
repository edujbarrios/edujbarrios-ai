#include "stratified_k_fold.h"

bool stratified_k_fold(const uint8_t *labels,size_t n,size_t k,uint32_t *folds){
    if (!labels || !folds || n<2 || n>4096 || k<2 || k>n) return false;
    size_t counts[2]={0,0};
    for (size_t i=0;i<n;++i) {
        if (labels[i]>1) return false;
        ++counts[labels[i]];
    }
    if (counts[0]<k || counts[1]<k) return false;
    size_t counter=0;
    for (uint8_t label=0;label<2;++label) {
        for (size_t i=0;i<n;++i) {
            if (labels[i]==label) {
                folds[i]=(uint32_t)(counter%k);
                ++counter;
            }
        }
    }
    return true;
}
