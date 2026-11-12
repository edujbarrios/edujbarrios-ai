#include "bernoulli_naive_bayes.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void close_to(double a, double b) { assert(fabs(a - b) < 1e-12); }

int main(void) {
    const uint8_t samples[] = {1,0, 1,1, 0,1, 0,0};
    const uint8_t labels[] = {0,0,1,1};
    const uint8_t q0[] = {1,0}, q1[] = {0,1};
    const uint8_t balanced[] = {0,0}, two_labels[] = {0,1}, qtie[] = {0};
    const uint8_t one_class[] = {0,0,0,0}, bad_label[] = {0,0,1,2};
    const uint8_t bad_feature[] = {1,0, 1,2, 0,1, 0,0}, bad_query[] = {1,2};
    uint8_t predicted = 9;
    double p = -1;
    assert(bernoulli_naive_bayes(samples, labels, 4, 2, q0, &predicted, &p));
    assert(predicted == 0); close_to(p, 0.25);
    assert(bernoulli_naive_bayes(samples, labels, 4, 2, q1, &predicted, &p));
    assert(predicted == 1); close_to(p, 0.75);
    assert(bernoulli_naive_bayes(balanced, two_labels, 2, 1, qtie, &predicted, &p));
    assert(predicted == 0); close_to(p, 0.5);

    uint8_t large[128 * 16] = {0}, large_labels[128] = {0}, query[16] = {0};
    for (size_t i = 64; i < 128; ++i) {
        large_labels[i] = 1;
        for (size_t j = 0; j < 16; ++j) large[i * 16 + j] = 1;
    }
    for (size_t j = 0; j < 16; ++j) query[j] = 1;
    assert(bernoulli_naive_bayes(large, large_labels, 128, 16, query, &predicted, &p));
    assert(predicted == 1 && p > 0.999);

    predicted = 9; p = -1;
    assert(!bernoulli_naive_bayes(NULL, labels, 4, 2, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, NULL, 4, 2, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, labels, 4, 0, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, labels, 4, 17, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, labels, 129, 2, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, one_class, 4, 2, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, bad_label, 4, 2, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(bad_feature, labels, 4, 2, q0, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, labels, 4, 2, bad_query, &predicted, &p));
    assert(!bernoulli_naive_bayes(samples, labels, 4, 2, q0, NULL, &p));
    assert(!bernoulli_naive_bayes(samples, labels, 4, 2, q0, &predicted, NULL));
    assert(predicted == 9 && p == -1);
    puts("C Bernoulli naive Bayes tests passed");
    return 0;
}
