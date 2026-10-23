#include "leaky_relu.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double input[] = {-2.0, 0.0, 3.0};
    const double invalid[] = {1.0, NAN};
    double output[] = {9.0, 9.0, 9.0};
    assert(leaky_relu(input, 3, 0.1, output));
    assert(fabs(output[0] + 0.2) < 1e-12);
    assert(output[1] == 0.0 && output[2] == 3.0);
    assert(leaky_relu(NULL, 0, 0.1, output));
    output[0] = 9.0;
    assert(!leaky_relu(input, 3, 0.0, output));
    assert(!leaky_relu(input, 3, 1.0, output));
    assert(!leaky_relu(invalid, 2, 0.1, output));
    assert(output[0] == 9.0);
    double inplace[] = {-4.0, 5.0};
    assert(leaky_relu(inplace, 2, 0.25, inplace));
    assert(inplace[0] == -1.0 && inplace[1] == 5.0);
    puts("C leaky_relu tests passed");
    return 0;
}
