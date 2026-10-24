#ifndef EDUJBARRIOS_AI_NEAREST_CENTROID_CLASSIFIER_H
#define EDUJBARRIOS_AI_NEAREST_CENTROID_CLASSIFIER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Train two class-major centroids from finite features[n*d], binary labels.
   n=2..4096,d=1..64, both classes required. All buffers disjoint.
   Fit failure preserves centroids[2*d]. */
bool nearest_centroid_classifier(const double *features,const uint8_t *labels,
                                 size_t n,size_t d,double *centroids);
/* Predict one finite query[d]; label output unchanged on failure. */
bool nearest_centroid_classifier_predict(const double *centroids,
             const double *query,size_t d,uint8_t *label);
#endif
