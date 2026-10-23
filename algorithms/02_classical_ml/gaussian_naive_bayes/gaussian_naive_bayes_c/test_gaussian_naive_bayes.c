#include "gaussian_naive_bayes.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {0,0, 0,1, 5,5, 5,6};
    const uint8_t y[] = {0,0,1,1};
    const double high[] = {5,5}, low[] = {0,0};
    const double tie_x[] = {0,2};
    const uint8_t tie_y[] = {0,1};
    const double tie_q[] = {1};
    const uint8_t wrong[] = {0,0,0,0};
    const uint8_t bad[] = {0,0,1,2};
    const double invalid[] = {NAN,0};
    uint8_t predicted = 9;
    double p1 = -1;
    assert(gaussian_naive_bayes(x,y,4,2,high,&predicted,&p1));
    assert(predicted == 1 && p1 > 0.99);
    assert(gaussian_naive_bayes(x,y,4,2,low,&predicted,&p1));
    assert(predicted == 0 && p1 < 0.01);
    assert(gaussian_naive_bayes(tie_x,tie_y,2,1,tie_q,&predicted,&p1));
    assert(predicted == 0 && fabs(p1-0.5)<1e-12);
    predicted=9; p1=-1;
    assert(!gaussian_naive_bayes(x,wrong,4,2,high,&predicted,&p1));
    assert(!gaussian_naive_bayes(x,bad,4,2,high,&predicted,&p1));
    assert(!gaussian_naive_bayes(invalid,y,2,1,tie_q,&predicted,&p1));
    assert(!gaussian_naive_bayes(x,y,4,0,high,&predicted,&p1));
    assert(!gaussian_naive_bayes(x,y,4,2,NULL,&predicted,&p1));
    assert(predicted == 9 && p1 == -1);
    puts("C gaussian naive bayes tests passed");
    return 0;
}
