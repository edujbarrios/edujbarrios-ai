#include "perceptron.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {-2,-1,-1,-1,1,1,2,1};
    const uint8_t y[] = {0,0,1,1};
    const double zero[] = {0,0};
    const uint8_t positive[] = {1};
    const uint8_t bad_labels[] = {0,0,1,2};
    const double nan_x[] = {NAN,0};
    const double huge[] = {1e308};
    const uint8_t one[] = {1};
    double w[] = {-9,-9}, bias = -9;
    uint8_t pred = 9;
    assert(perceptron(x,y,4,2,1.0,1,w,&bias));
    assert(w[0] == 1 && w[1] == 1 && bias == 1);
    assert(perceptron_predict(w,x,2,bias,&pred) && pred == 0);
    assert(perceptron_predict(w,x+6,2,bias,&pred) && pred == 1);
    assert(perceptron(zero,positive,1,2,1.0,1,w,&bias));
    assert(w[0] == 0 && w[1] == 0 && bias == 1);
    const double tie_w[] = {0,0};
    assert(perceptron_predict(tie_w,zero,2,0,&pred) && pred == 0);
    w[0]=-9;w[1]=-9;bias=-9;
    assert(!perceptron(x,bad_labels,4,2,1,1,w,&bias));
    assert(!perceptron(nan_x,positive,1,2,1,1,w,&bias));
    assert(!perceptron(x,y,4,2,0,1,w,&bias));
    assert(!perceptron(x,y,4,2,1,0,w,&bias));
    assert(!perceptron(x,y,0,2,1,1,w,&bias));
    assert(!perceptron(huge,one,1,1,1e308,2,w,&bias));
    assert(!perceptron_predict(tie_w,nan_x,2,0,&pred));
    assert(w[0]==-9 && w[1]==-9 && bias==-9);
    puts("C perceptron tests passed");
    return 0;
}
