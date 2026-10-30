#include "binary_opening.h"
#include <stdlib.h>

bool binary_opening(const uint8_t *image, size_t n, size_t width,
                    size_t height, uint8_t *output) {
    if (!image || !output || width==0 || height==0 || width>64 || height>64 ||
        n!=width*height) return false;
    for (size_t i=0; i<n; ++i) if (image[i]>1) return false;
    uint8_t *eroded = malloc(n * sizeof(*eroded));
    if (!eroded) return false;
    for (size_t y=0; y<height; ++y) {
        for (size_t x=0; x<width; ++x) {
            uint8_t keep=1;
            for (int dy=-1; dy<=1; ++dy) {
                for (int dx=-1; dx<=1; ++dx) {
                    long ny=(long)y+dy, nx=(long)x+dx;
                    if (ny<0 || nx<0 || ny>=(long)height || nx>=(long)width ||
                        image[(size_t)ny*width+(size_t)nx]==0) keep=0;
                }
            }
            eroded[y*width+x]=keep;
        }
    }
    for (size_t y=0; y<height; ++y) {
        for (size_t x=0; x<width; ++x) {
            uint8_t keep=0;
            for (int dy=-1; dy<=1; ++dy) {
                for (int dx=-1; dx<=1; ++dx) {
                    long ny=(long)y+dy, nx=(long)x+dx;
                    if (ny>=0 && nx>=0 && ny<(long)height && nx<(long)width &&
                        eroded[(size_t)ny*width+(size_t)nx]==1) keep=1;
                }
            }
            output[y*width+x]=keep;
        }
    }
    free(eroded);
    return true;
}
